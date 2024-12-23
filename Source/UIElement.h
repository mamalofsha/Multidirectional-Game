#pragma once
#include "Object.h"
class UIElement : public Object {
public:
    float x, y; // Center position
    float width, height; // Dimensions
    unsigned int VBO, VAO, EBO;
    unsigned int Texture;

    UIElement(std::shared_ptr<Shader> shaderProgram, float x, float y, float width, float height)
        : Object(shaderProgram), x(x), y(y), width(width), height(height) {

    }

    virtual void Draw() = 0; // Pure virtual function for rendering
    virtual void Update(float mouseX, float mouseY, bool isClicked) {
        // Default implementation for updating hover/click states
    }


    bool isInside(float mouseX, float mouseY) const {
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        return mouseX >= x - halfWidth && mouseX <= x + halfWidth &&
            mouseY <= y + halfHeight && mouseY >= y - halfHeight;
    }

    void InitializeFromRenderData(const RenderData& data)override
    {
        VAO = data.VAO;
        VBO = data.VBO;
        EBO = data.EBO;
        Texture = data.TextureID;
    }
};
