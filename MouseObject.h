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
        : TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData,false,InWorldPtr) {
        // Additional MouseObject-specific initialization here
    }
    void draw() override {
       ObjectShader->use();
        
        MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));

        // Convert mouse position to NDC
        auto [ndcX, ndcY] = api->screenToNDC(api->GetMouseState().x, api->GetMouseState().y, WorldPtr->GetWindowSize()[0], WorldPtr->GetWindowSize()[1]);

        // Use shader program

        ObjectShader->setUniform2f("mousePos", ndcX, ndcY);
        ObjectShader->setFloat("size", size);
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
};