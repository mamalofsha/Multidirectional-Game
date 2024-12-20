#pragma once
#include <functional>
#include <GLFW/glfw3.h>

class Button {
public:
    std::unique_ptr<Shader> ObjectShader;
    float x, y, width, height;
    bool isHovered = false;
    std::function<void()> onClick;


    Button(float x, float y, float width, float height, std::function<void()> onClick)
        : x(x), y(y), width(width), height(height), onClick(onClick) {
    }


    void draw() {
        ObjectShader->use();
        glBindVertexArray(ObjectShader->VAO);

        // Change color if hovered
        if (isHovered) {
            int vertexColorLocation = glGetUniformLocation(ObjectShader->ID, "hoverColor");
            glUniform3f(vertexColorLocation, 0.7f, 0.7f, 0.9f); // Light blue
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    bool isInside(float mouseX, float mouseY) {
        return mouseX >= x && mouseX <= x + width && mouseY <= y && mouseY >= y - height;
    }
};
