#pragma once
#include "TexturedObject.h"
#include "MouseInteraction.h"
#include "Building.h"
#include "Decoration.h"
#include "WorkShop.h"

class MouseObject : public TexturedObject
{
private :
	bool isAttachedToGrid;
	std::string ItemID;
	const char* cachedfilePath;
public:
	MouseObject(std::shared_ptr<Shader> shaderProgram,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const char* texturePath,
		const VertexAttribute& vertexData,
		World* InWorldPtr)
		: TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData, InWorldPtr) , cachedfilePath(texturePath){
		// Additional MouseObject-specific initialization here
		IsHidden = true;

	}
	void Draw() override {

		if (IsHidden)return;
		ObjectShader->use();

		float ndcX = 0.0f;
		float ndcY = 0.0f;
		float screenX = 0.0f;
		float screenY = 0.0f;
		auto [winX, winY] = WorldPtr->GetWindowSize();
		MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
		if (api->GetMouseState().GridX >= 0 && api->GetMouseState().GridX < WorldPtr->GetGridConfig().Width &&
			api->GetMouseState().GridY >= 0 && api->GetMouseState().GridY < WorldPtr->GetGridConfig().Height)
		{
			std::string gridvalue = XMLParser::GetGridValue(WorldPtr->GetStartupData().GridFileName, api->GetMouseState().GridX, api->GetMouseState().GridY);
			if (gridvalue != "0")
			{
				SetSize(0.05f);
				std::tie(ndcX, ndcY) = api->screenToNDC(api->GetMouseState().x, api->GetMouseState().y, winX, winY);
				isAttachedToGrid = false;
				ObjectShader->setBool("isOverlapping", true);

			}
			else
			{
				ObjectShader->setBool("isOverlapping", false);

				SetSize(0.15f);
				auto [winX, winY] = WorldPtr->GetWindowSize();

				std::tie(screenX, screenY) = Graphics::GridToWorldPosition(api->GetMouseState().GridX, api->GetMouseState().GridY,
					WorldPtr->GetGridConfig().TileWidth, WorldPtr->GetGridConfig().TileHeight,
					WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPan().first, WorldPtr->GetPan().second, Size, WorldPtr->GetZoom(), winX, winY, WorldPtr->GetLevelSize().first, WorldPtr->GetLevelSize().second);
				std::tie(ndcX, ndcY) = api->screenToNDC(screenX, screenY, winX, winY);
				isAttachedToGrid = true;
			}

		}
		else
		{
			ObjectShader->setBool("isOverlapping", false);

			SetSize(0.05f);
			std::tie(ndcX, ndcY) = api->screenToNDC(api->GetMouseState().x, api->GetMouseState().y, winX, winY);
			isAttachedToGrid = false;
		}

		float scaleX = WorldPtr->GetLevelSize().first / winX;
		float scaleY = WorldPtr->GetLevelSize().second / winY;
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(ndcX, ndcY, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom()*Size, scaleY * WorldPtr->GetZoom() * Size, 1.0f));
		ObjectShader->setMat4("transform", transform);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Draw quad
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void SetItemID(const std::string InItemID) { ItemID = InItemID; };

	void ReloadTexture(const char* filePath)
	{
		glBindTexture(GL_TEXTURE_2D, Texture);
		cachedfilePath = filePath;
		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load the image
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
		if (data) {
			GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB; // Detect if image has alpha
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "Loaded texture: " << filePath << std::endl;
		}
		else {
			std::cerr << "Failed to load texture: " << filePath << std::endl;
		}
		SetHidden(false);
		stbi_image_free(data);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool GetIsAttachedtoGrid() { return isAttachedToGrid; };

	void ExecuteAction() 
	{
		std::cout << "decdi";
		if (GetIsAttachedtoGrid())
		{
			SetHidden(true);
			// Vertex data with texture coordinates
			std::vector<float> vertices = {
				// Positions      // Texture Coords
				-0.5f, -0.5f,     0.0f, 0.0f, // Bottom-left
				 0.5f, -0.5f,     1.0f, 0.0f, // Bottom-right
				 0.5f,  0.5f,     1.0f, 1.0f, // Top-right
				-0.5f,  0.5f,     0.0f, 1.0f  // Top-left
			};

			std::vector<unsigned int> indices = {
				0, 1, 2, // First triangle
				2, 3, 0  // Second triangle
			};
			VertexAttribute OutVertexData = { 4,{2,2} };

			MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
			WorkshopData TempWorkShopData = XMLParser::LoadWorkshop("ShopItems.xml", "workshops", ItemID);
			if (!TempWorkShopData.Name.empty())
			{
				WorldPtr->Buildings.emplace_back(std::make_unique<Workshop>(ObjectShader, vertices, indices, OutVertexData, WorldPtr, api->GetMouseState().GridX, api->GetMouseState().GridY, TempWorkShopData));
			}
			else
			{
				DecorationData TempDecorationData = XMLParser::LoadDecoration("ShopItems.xml", "decorations", ItemID);
				if (!TempDecorationData.Name.empty())
				{
					WorldPtr->Buildings.emplace_back(std::make_unique<Decoration>(ObjectShader, vertices, indices, OutVertexData, WorldPtr, api->GetMouseState().GridX, api->GetMouseState().GridY, TempDecorationData));
				}
			}
			XMLParser::UpdateGridValue(WorldPtr->GetStartupData().GridFileName, api->GetMouseState().GridX, api->GetMouseState().GridY, ItemID.c_str());

		}
	}
};