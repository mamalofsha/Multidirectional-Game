#pragma once
#include "TexturedObject.h"

class Building : public TexturedObject
{
protected:
    int GridX,GridY;

public:
	Building(std::shared_ptr<Shader> shaderProgram,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const char* texturePath,
		const VertexAttribute& vertexData,
		World* InWorldPtr, int InGridX, int InGridY)
		: TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData, InWorldPtr), GridX(InGridX), GridY(InGridY){
		// Additional MouseObject-specific initialization here
		setSize(0.15f);
		/// fix it , it breaks the ui but everyone here counts from 0
	}
	void draw() override {

		
		if (isHidden)return;
		ObjectShader->use();
		ObjectShader->setBool("isOverlapping", false);

		float ndcX = 0.0f;
		float ndcY = 0.0f;
		float screenX = 0.0f;
		float screenY = 0.0f;
		auto [winX, winY] = WorldPtr->GetWindowSize();
		MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));

		std::tie(screenX, screenY) = Graphics::GridToWorldPosition(GridX, GridY,
		WorldPtr->GetGridConfig().tileWidth, WorldPtr->GetGridConfig().tileHeight,
		WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPan().first, WorldPtr->GetPan().second, size, WorldPtr->GetZoom(), winX, winY);
		std::tie(ndcX, ndcY) = api->screenToNDC(screenX, screenY, winX, winY);




		float scaleX = WorldPtr->GetLevelSize().first / winX;
		float scaleY = WorldPtr->GetLevelSize().second / winY;
		glm::mat4 transform = glm::mat4(1.0f);
		GLuint  transformLoc;
		transform = glm::translate(transform, glm::vec3(ndcX, ndcY, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom() * size, scaleY * WorldPtr->GetZoom() * size, 1.0f));

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
};