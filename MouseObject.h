#pragma once
#include "TexturedObject.h"
#include "MouseInteraction.h"

class MouseObject : public TexturedObject
{
public:
	MouseObject(std::shared_ptr<Shader> shaderProgram,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const char* texturePath,
		const VertexAttribute& vertexData,
		World* InWorldPtr)
		: TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData, false, InWorldPtr) {
		// Additional MouseObject-specific initialization here
		isHidden = true;
	}
	void draw() override {

		if (isHidden)return;
		ObjectShader->use();

		MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));

		// Convert mouse position to NDC
	   // auto [ndcX, ndcY] = api->screenToNDC(api->GetMouseState().x, api->GetMouseState().y, WorldPtr->GetWindowSize()[0], WorldPtr->GetWindowSize()[1]);
		// Use shader program
	//	auto [screenX, screenY] = Graphics::GridToWorldPosition(api->GetMouseState().GridX, api->GetMouseState().GridY,
	//		WorldPtr->GetGridConfig().tileWidth, WorldPtr->GetGridConfig().tileHeight,
	//		WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPanX(), WorldPtr->GetPanY(), WorldPtr->GetZoom(), WorldPtr->GetWindowSize()[0], WorldPtr->GetWindowSize()[1]);
	
		auto [screenX, screenY] = Graphics::GridToWorldPosition(api->GetMouseState().GridX, api->GetMouseState().GridY,
			WorldPtr->GetGridConfig().tileWidth, WorldPtr->GetGridConfig().tileHeight,
			WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPanX(), WorldPtr->GetPanY(), WorldPtr->GetZoom(), WorldPtr->GetWindowSize()[0], WorldPtr->GetWindowSize()[1]);
		auto [ndcX, ndcY] = api->screenToNDC(screenX, screenY, WorldPtr->GetWindowSize()[0], WorldPtr->GetWindowSize()[1]);
		
		//system("cls");
		std::cout << "scx:" << screenX << " ndx: " << ndcX << " x "<<api->GetMouseState().x<< std::endl;
		 std::cout << "scy:" << screenY << " ndy: " << ndcY <<" y " << api->GetMouseState().y << std::endl;


		ObjectShader->setFloat("size", size);

		std::vector<int> WindowSize = WorldPtr->GetWindowSize();
		float scaleX = 2000.0f / WindowSize[0];
		float scaleY = 1404.0f / WindowSize[1];
		glm::mat4 transform = glm::mat4(1.0f);
		GLuint  transformLoc;
		transform = glm::translate(transform, glm::vec3(ndcX, ndcY, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));

		transformLoc = glGetUniformLocation(ObjectShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		// Set uniform values
		std::cout << ndcY << std::endl;
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
};