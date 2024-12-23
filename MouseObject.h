#pragma once
#include "TexturedObject.h"
#include "MouseInteraction.h"
#include "Building.h"

class MouseObject : public TexturedObject
{
private :
	bool isAttachedToGrid;
	const char* cachedfilePath;
public:
	MouseObject(std::shared_ptr<Shader> shaderProgram,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const char* texturePath,
		const VertexAttribute& vertexData,
		World* InWorldPtr)
		: TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData, false, InWorldPtr) , cachedfilePath(texturePath){
		// Additional MouseObject-specific initialization here
		isHidden = true;

	}
	void draw() override {

		if (isHidden)return;
		ObjectShader->use();

		float ndcX = 0.0f;
		float ndcY = 0.0f;
		float screenX = 0.0f;
		float screenY = 0.0f;
		auto [winX, winY] = WorldPtr->GetWindowSize();
		MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
		if (api->GetMouseState().GridX >= 1 && api->GetMouseState().GridX <= WorldPtr->GetGridConfig().width &&
			api->GetMouseState().GridY >= 1 && api->GetMouseState().GridY <= WorldPtr->GetGridConfig().height)
		{
			setSize(0.15f);
			auto [winX, winY] = WorldPtr->GetWindowSize();

			std::tie(screenX, screenY) = Graphics::GridToWorldPosition(api->GetMouseState().GridX, api->GetMouseState().GridY,
				WorldPtr->GetGridConfig().tileWidth, WorldPtr->GetGridConfig().tileHeight,
				WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPanX(), WorldPtr->GetPanY(), size, WorldPtr->GetZoom(), winX, winY);
			std::tie(ndcX, ndcY) = api->screenToNDC(screenX, screenY, winX, winY);
			isAttachedToGrid = true;
		}
		else
		{
			setSize(0.05f);
			std::tie(ndcX, ndcY) = api->screenToNDC(api->GetMouseState().x, api->GetMouseState().y, winX, winY);
			isAttachedToGrid = false;
		}


		float scaleX = 2000.0f / winX;
		float scaleY = 1404.0f / winY;
		glm::mat4 transform = glm::mat4(1.0f);
		GLuint  transformLoc;
		transform = glm::translate(transform, glm::vec3(ndcX, ndcY, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom()*size, scaleY * WorldPtr->GetZoom() * size, 1.0f));

		transformLoc = glGetUniformLocation(ObjectShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		// Set uniform values
		//std::cout << ndcY << std::endl;
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, Texture);

		// Draw quad
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

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
			MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));

			VertexAttribute OutVertexData = { 4,{2,2} };
			WorldPtr->builds.emplace_back(std::make_unique<Building>(ObjectShader, vertices, indices, cachedfilePath, OutVertexData, WorldPtr, api->GetMouseState().GridX, api->GetMouseState().GridY));
		}
	}
};