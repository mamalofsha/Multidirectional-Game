#pragma once

class UIElement {
public:
    std::unique_ptr<Shader> ObjectShader;
    float x, y; // Center position
    float width, height; // Dimensions
    bool isHidden = false; // Visibility flag

    UIElement(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {
    }

    virtual void draw() = 0; // Pure virtual function for rendering
    virtual void update(float mouseX, float mouseY, bool isClicked) {
        // Default implementation for updating hover/click states
    }
    virtual void SetHidden(bool newHidden)
    {
        isHidden = newHidden;
    }

    bool isInside(float mouseX, float mouseY) const {
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        return mouseX >= x - halfWidth && mouseX <= x + halfWidth &&
            mouseY <= y + halfHeight && mouseY >= y - halfHeight;
    }
};
