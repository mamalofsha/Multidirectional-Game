#pragma once
#include "Object.h"

struct UIText
{
    float OffSetX = 0.0f;
    float OffSetY = 0.0f;
    std::string ActualText = "";
    float Scale = 1.0f;
    glm::vec3 TextColor = glm::vec3(0, 0, 0);
};

class UIElement : public Object {
public:
    float PosX, PosY; // Center position
    float Width, Height; // Dimensions
    unsigned int VBO, VAO, EBO;
    unsigned int Texture;
    std::vector<UIText> TextComps;
    class HUD* HudPtr;

    UIElement(std::shared_ptr<Shader> InShaderProgram, float InX, float InY, float InWidth, float InHeight)
        : Object(InShaderProgram), PosX(InX), PosY(InY), Width(InWidth), Height(InHeight) {
    }
    virtual void Draw() = 0;
    void DrawText();
    virtual void Update(float InMouseX, float InMouseY, bool InIsClicked) {}
    bool IsInside(float InMouseX, float InMouseY) const;
    void InitializeFromRenderData(const RenderData& InData) override;
};
