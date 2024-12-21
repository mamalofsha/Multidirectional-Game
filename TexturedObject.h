#pragma once
#include "Object.h"
#include "stb_image.h"

class TexturedObject : public Object {
private:
    unsigned int VAO, VBO, EBO; // OpenGL handles
    unsigned int texture;       // Texture handle
    glm::vec2 position;
    float size;

public:
    TexturedObject(std::shared_ptr<Shader> shaderProgram, float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize, const char* texturePath)
        : Object(shaderProgram) {
        // OpenGL setup (VAO, VBO, EBO, texture loading, etc.)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

        // Configure vertex attributes
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Load texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
        }
        stbi_image_free(data);
    }

    ~TexturedObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture);
    }

    void setPosition(float x, float y) {
        position = glm::vec2(x, y);
    }

    void setSize(float s) {
        size = s;
    }

    void draw() override {
       /// shader->use();

        // Set uniforms
        glUniform2f(glGetUniformLocation(shader->ID, "mousePos"), position.x, position.y);
        glUniform1f(glGetUniformLocation(shader->ID, "size"), size);

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Bind VAO and draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};
