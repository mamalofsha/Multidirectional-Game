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
        InitializeFromRenderData(Graphics::DrawTexture(vertices, indices, InAttribute, texturePath));
    }
    ~TexturedObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &Texture);
    }
    void Draw() override;
    void InitializeFromRenderData(const RenderData& InData) override;
};
