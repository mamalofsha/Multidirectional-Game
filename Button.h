#pragma once
#include <functional>
#include <GLFW/glfw3.h>

class Button {
public:
    float x, y, width, height;
    bool isHovered = false;
    std::function<void()> onClick;

    unsigned int VAO, VBO, shaderProgram;

    Button(float x, float y, float width, float height, std::function<void()> onClick, unsigned int shaderProgram)
        : x(x), y(y), width(width), height(height), onClick(onClick), shaderProgram(shaderProgram) {
        setupButton();
    }

    void setupButton() {
        // Define vertex data for the button (rectangle)
        float vertices[] = {
            // Positions    // Colors (default blue)
            x, y, 0.0f, 0.0f, 1.0f, // Top-left
            x + width, y, 0.0f, 0.0f, 1.0f, // Top-right
            x + width, y - height, 0.0f, 0.0f, 1.0f, // Bottom-right
            x, y - height, 0.0f, 0.0f, 1.0f // Bottom-left
        };

        unsigned int indices[] = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

        // Generate VAO and VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Change color if hovered
        if (isHovered) {
            int vertexColorLocation = glGetUniformLocation(shaderProgram, "hoverColor");
            glUniform3f(vertexColorLocation, 0.7f, 0.7f, 0.9f); // Light blue
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    bool isInside(float mouseX, float mouseY) {
        return mouseX >= x && mouseX <= x + width && mouseY <= y && mouseY >= y - height;
    }
};
