#pragma once
#include "Object.h"
#include "stb_image.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "World.h"

class TexturedObject : public Object {
private:
    unsigned int VAO, VBO, EBO; // OpenGL handles
    unsigned int Texture;       // Texture handle
    glm::vec2 position;
    float size;
    World* WorldPtr;
public:
    TexturedObject(std::shared_ptr<Shader> shaderProgram, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const char* texturePath,World* InWorldptr)
        : Object(shaderProgram) {
        // OpenGL setup (VAO, VBO, EBO, texture loading, etc.)
        // Create VAO, VBO, and EBO
        WorldPtr = InWorldptr;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        // Load texture
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);

        // Texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
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
        float scaleX = 2000.0f / WindowSize[0];
        float scaleY = 1404.0f / WindowSize[1];
        glm::mat4 transform = glm::mat4(1.0f);
        GLuint  transformLoc;
        glBindTexture(GL_TEXTURE_2D, Texture);
        transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));
        transform = glm::translate(transform, glm::vec3(WorldPtr->GetPanX(), WorldPtr->GetPanY(), 0.0f));

        transformLoc = glGetUniformLocation(ObjectShader->ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));



        //it->shader.setFloat("zoom", Zoom);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }
};
