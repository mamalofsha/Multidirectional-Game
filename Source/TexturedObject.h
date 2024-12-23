#pragma once
#include "Object.h"
#include "stb_image.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "World.h"

class TexturedObject : public Object {
protected:
    unsigned int VAO, VBO, EBO; // OpenGL handles
    unsigned int Texture;       // Texture handle

public:
    TexturedObject(std::shared_ptr<Shader> shaderProgram, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const char* texturePath, VertexAttribute InAttribute,World* InWorldptr)
        : Object(shaderProgram) {
        WorldPtr = InWorldptr;
        RenderData NewData = Graphics::DrawTexture(vertices,indices, InAttribute,texturePath);
        VAO = NewData.VAO;
        VBO = NewData.VBO;
        EBO = NewData.EBO;
        Texture = NewData.TextureID;
    }

    ~TexturedObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &Texture);
    }

    void draw() override {
        auto [winX,winY] = WorldPtr->GetWindowSize();
        float scaleX = WorldPtr->GetLevelSize().first / winX;
        float scaleY = WorldPtr->GetLevelSize().second / winY;
        glm::mat4 transform = glm::mat4(1.0f);
        glBindTexture(GL_TEXTURE_2D, Texture);
        transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));
        transform = glm::translate(transform, glm::vec3(WorldPtr->GetPan().first, WorldPtr->GetPan().second, 0.0f));
        ObjectShader->setMat4("transform", transform);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
};
