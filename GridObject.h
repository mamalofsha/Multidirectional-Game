#pragma once
#include "Object.h"
#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

class GridObject : public Object
{
private:
    unsigned int VAO, VBO; // OpenGL handles
    glm::vec2 position;
    size_t gridVerticesSize;
    World* WorldPtr;
public:
    GridObject(std::shared_ptr<Shader> shaderProgram, GridConfig InGridConfig, World* InWorldptr)
        : Object(shaderProgram) {
        // OpenGL setup (VAO, VBO, EBO, texture loading, etc.)
        // Create VAO, VBO, and EBO
        WorldPtr = InWorldptr;

		std::vector<float> gridVertices = Graphics::createGridVertices(InGridConfig.width, InGridConfig.height, InGridConfig.StartOffsetX, InGridConfig.StartOffsetY);
        gridVerticesSize = gridVertices.size();
		// Create vertex buffer and array objects
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, gridVerticesSize * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		ObjectShader->use();

		// Set uniform values
		GLint tileSizeLocation = glGetUniformLocation(ObjectShader->ID, "tileSize");
		glUniform2f(tileSizeLocation, InGridConfig.tileWidth, InGridConfig.tileHeight);
		GLint screenSizeLocation = glGetUniformLocation(ObjectShader->ID, "screenSize");
		auto [winX,winY]= WorldPtr->GetWindowSize();
		glUniform2f(screenSizeLocation, winX, winY);
    }

    ~GridObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void draw() override {
		auto [winX, winY] = WorldPtr->GetWindowSize();
        float scaleX = 2000.0f / winX;
        float scaleY = 1404.0f / winY;
		glm::mat4 transform = glm::mat4(1.0f);
		GLuint  transformLoc;
		ObjectShader->setUniform2i("tileCoor", WorldPtr->GetMouserState().GridX, WorldPtr->GetMouserState().GridY);

		transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));
		transform = glm::translate(transform, glm::vec3(WorldPtr->GetPanX(), WorldPtr->GetPanY(), 0.0f));

		transformLoc = glGetUniformLocation(ObjectShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		//// todoooo donmt forgetteettete it shoudl be grid vertices size gridvertices.sizer() /2 
		glDrawArrays(GL_LINES, 0, gridVerticesSize / 2);

    }
};