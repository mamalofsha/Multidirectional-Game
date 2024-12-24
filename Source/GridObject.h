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
    unsigned int VAO, VBO; 
    size_t GridVerticesSize;
public:
    GridObject(std::shared_ptr<Shader> InShaderProgram, GridConfig InGridConfig, World* InWorldPtr)
        : Object(InShaderProgram)
    {
        WorldPtr = InWorldPtr;
        std::vector<float> GridVertices = Graphics::CreateGridVertices(InGridConfig.Width, InGridConfig.Height, 
                                                                       InGridConfig.StartOffsetX, InGridConfig.StartOffsetY);
        GridVerticesSize = GridVertices.size();
        ObjectShader->use();
        ObjectShader->setUniform2f("TileSize", InGridConfig.TileWidth, InGridConfig.TileHeight);
        auto [WindowWidth, WindowHeight] = WorldPtr->GetWindowSize();
        ObjectShader->setUniform2f("ScreenSize", WindowWidth, WindowHeight);
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, GridVerticesSize * sizeof(float), GridVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    ~GridObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

	void Draw() override;
};