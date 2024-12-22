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
    glm::vec2 position;
    float size;
    World* WorldPtr;
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

    void setPosition(float x, float y) {
        position = glm::vec2(x, y);
    }

    void setSize(float s) {
        size = s;
    }

    void draw() override {
        std::vector<int> WindowSize = WorldPtr->GetWindowSize();
        ObjectShader->use();
        // todo dont forget to read from xmll
        float scaleX = 2000.0f / WindowSize[0];
        float scaleY = 1404.0f / WindowSize[1];
        glm::mat4 transform = glm::mat4(1.0f);
        GLuint  transformLoc;
        glBindTexture(GL_TEXTURE_2D, Texture);
        transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));
        transform = glm::translate(transform, glm::vec3(WorldPtr->GetPanX(), WorldPtr->GetPanY(), 0.0f));
        transformLoc = glGetUniformLocation(ObjectShader->ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
