#include "MouseObject.h"
#include "Building.h"
#include "Decoration.h"
#include "WorkShop.h"
#include "MouseInteraction.h"

void MouseObject::Draw()
{
	if (IsHidden) return;
	ObjectShader->use();
	float NdcX = 0.0f;
	float NdcY = 0.0f;
	float ScreenX = 0.0f;
	float ScreenY = 0.0f;
	auto [WinX, WinY] = WorldPtr->GetWindowSize();
	MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
	// checking if we're in grid
	if (Api->GetMouseState().GridX >= 0 && Api->GetMouseState().GridX < WorldPtr->GetGridConfig().Width &&
		Api->GetMouseState().GridY >= 0 && Api->GetMouseState().GridY < WorldPtr->GetGridConfig().Height)
	{
		// checking if  grid is occupied
		std::string GridValue = XMLParser::GetGridValue(WorldPtr->GetStartupData().GridFileName, Api->GetMouseState().GridX, Api->GetMouseState().GridY);
		if (GridValue != "0")
		{
			SetSize(0.05f);
			std::tie(NdcX, NdcY) = Api->ScreenToNDC(Api->GetMouseState().X, Api->GetMouseState().Y, WinX, WinY);
			IsAttachedToGrid = false;
			ObjectShader->setBool("IsOverlapping", true);
			IsOverlapping = true;
		}
		else
		{
			ObjectShader->setBool("IsOverlapping", false);
			IsOverlapping = false;
			SetSize(0.15f);
			std::tie(ScreenX, ScreenY) = Graphics::GridToWorldPosition(Api->GetMouseState().GridX, Api->GetMouseState().GridY,
				WorldPtr->GetGridConfig().TileWidth, WorldPtr->GetGridConfig().TileHeight,
				WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY,
				WorldPtr->GetPan().first, WorldPtr->GetPan().second, Size, WorldPtr->GetZoom(),
				WinX, WinY, WorldPtr->GetLevelSize().first, WorldPtr->GetLevelSize().second);
			std::tie(NdcX, NdcY) = Api->ScreenToNDC(ScreenX, ScreenY, WinX, WinY);
			IsAttachedToGrid = true;
		}
	}
	else
	{
		ObjectShader->setBool("IsOverlapping", false);
		IsOverlapping = false;
		SetSize(0.05f);
		std::tie(NdcX, NdcY) = Api->ScreenToNDC(Api->GetMouseState().X, Api->GetMouseState().Y, WinX, WinY);
		IsAttachedToGrid = false;
	}
	float ScaleX = WorldPtr->GetLevelSize().first / WinX;
	float ScaleY = WorldPtr->GetLevelSize().second / WinY;
	glm::mat4 Transform = glm::mat4(1.0f);
	Transform = glm::translate(Transform, glm::vec3(NdcX, NdcY, 0.0f));
	Transform = glm::scale(Transform, glm::vec3(ScaleX * WorldPtr->GetZoom() * Size, ScaleY * WorldPtr->GetZoom() * Size, 1.0f));
	ObjectShader->setMat4("Transform", Transform);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MouseObject::ReloadTexture(const char* InFilePath)
{
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load the image
	int Width, Height, NrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load(InFilePath, &Width, &Height, &NrChannels, 0);
	if (Data) {
		GLenum Format = (NrChannels == 4) ? GL_RGBA : GL_RGB; // Detect if image has alpha
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Loaded texture: " << InFilePath << std::endl;
	}
	else {
		std::cerr << "Failed to load texture: " << InFilePath << std::endl;
	}
	SetHidden(false);
	stbi_image_free(Data);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MouseObject::ExecuteAction()
{
	// spawn building 
	if (GetIsAttachedToGrid())
	{
		std::vector<float> Vertices = {
			// Positions      // Texture Coords
			-0.5f, -0.5f,     0.0f, 0.0f, // Bottom-left
			 0.5f, -0.5f,     1.0f, 0.0f, // Bottom-right
			 0.5f,  0.5f,     1.0f, 1.0f, // Top-right
			-0.5f,  0.5f,     0.0f, 1.0f  // Top-left
		};
		std::vector<unsigned int> Indices = {
			0, 1, 2, // First triangle
			2, 3, 0  // Second triangle
		};
		VertexAttribute OutVertexData = { 4,{2,2} };
		MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
		WorkshopData TempWorkShopData = XMLParser::LoadWorkshop("ShopItems.xml", "workshops", ItemID);
		if (!TempWorkShopData.Name.empty())
		{
			WorldPtr->Buildings.emplace_back(std::make_unique<Workshop>(ObjectShader, Vertices, Indices, OutVertexData, WorldPtr, Api->GetMouseState().GridX, Api->GetMouseState().GridY, TempWorkShopData));
		}
		else
		{
			DecorationData TempDecorationData = XMLParser::LoadDecoration("ShopItems.xml", "decorations", ItemID);
			if (!TempDecorationData.Name.empty())
			{
				WorldPtr->Buildings.emplace_back(std::make_unique<Decoration>(ObjectShader, Vertices, Indices, OutVertexData, WorldPtr, Api->GetMouseState().GridX, Api->GetMouseState().GridY, TempDecorationData));
			}
		}
		XMLParser::UpdateGridValue(WorldPtr->GetStartupData().GridFileName, Api->GetMouseState().GridX, Api->GetMouseState().GridY, ItemID.c_str());
	}
	if (!IsOverlapping)
		SetHidden(true);
}
