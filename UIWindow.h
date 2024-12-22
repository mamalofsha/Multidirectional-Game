#pragma once
#include "UIElement.h"
#include <vector>
#include <memory>

class UIWindow : public UIElement {
public:
    std::vector<std::shared_ptr<UIElement>> children;

    UIWindow(std::shared_ptr<Shader> shaderProgram, float x, float y, float width, float height)
        : UIElement(shaderProgram,x, y, width, height) {
    }

    void draw() override {
        if (isHidden) return;

        // Render the window background
        // Example: Use a shader for window appearance
        ObjectShader->use();
        // Draw the window as a rectangle or textured quad

        for (auto& child : children) {
            child->draw();
        }
    }



    void update(float mouseX, float mouseY, bool isClicked) override {
        if (isHidden) return;

        for (auto& child : children) {
            child->update(mouseX, mouseY, isClicked);
        }
    }

    void addChild(std::shared_ptr<UIElement> child) {
        children.push_back(child);
    }

    virtual void SetHidden(bool newHidden)override
    {
        isHidden = newHidden;
        for (auto& child : children) {
            child->SetHidden(newHidden);
        }
    }
};
