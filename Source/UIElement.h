#pragma once
#include "Object.h"
class UIElement : public Object {
public:
    float PosX, PosY; // Center position
    float Width, Height; // Dimensions
    unsigned int VBO, VAO, EBO;
    unsigned int Texture;

    UIElement(std::shared_ptr<Shader> InShaderProgram, float InX, float InY, float InWidth, float InHeight)
        : Object(InShaderProgram), PosX(InX), PosY(InY), Width(InWidth), Height(InHeight) {
    }
    virtual void Draw() = 0;
    virtual void Update(float InMouseX, float InMouseY, bool InIsClicked) {}
    bool IsInside(float InMouseX, float InMouseY) const;
    void InitializeFromRenderData(const RenderData& InData) override;
};
