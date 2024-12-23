#pragma once
#include <functional>
#include "UIElement.h"
#include "HUD.h"

class UIButton : public UIElement {
public:
    bool IsHovered = false;
    std::function<void()> OnClick;
    std::string Text;
    HUD* HudPtr;

    UIButton(std::shared_ptr<Shader> InShaderProgram, float InX, float InY, float InWidth, float InHeight,
        std::function<void()> InOnClick, const std::string& InText, const std::string& InAssetPath, HUD* InHudPtr)
        : UIElement(InShaderProgram, InX, InY, InWidth, InHeight), OnClick(InOnClick) {
        HudPtr = InHudPtr;
        Text = InText;
        InitializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{InX, InY},
            std::vector<float>{InWidth, InHeight}, InAssetPath.c_str()));
    }
    void UpdateHoverState(double InMouseX, double InMouseY) { IsHovered = IsInside(InMouseX, InMouseY); }
    void Clicked() {
        std::cout << Text;
    }
    void Draw();
};
