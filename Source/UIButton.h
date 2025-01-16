#pragma once
#include <functional>
#include "UIElement.h"
#include "HUD.h"

class UIButton : public UIElement {
public:
    bool IsHovered = false;
    std::function<void()> OnClick;
    bool HoverOnlyText = false;
    UIButton(std::shared_ptr<Shader> InShaderProgram, float InX, float InY, float InWidth, float InHeight,
        std::function<void()> InOnClick, const std::string& InAssetPath, HUD* InHudPtr, std::vector<UIText> InTextInfo = {})
        : UIElement(InShaderProgram, InX, InY, InWidth, InHeight), OnClick(InOnClick){
        HudPtr = InHudPtr;
        InitializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{InX, InY},
            std::vector<float>{InWidth, InHeight}, InAssetPath.c_str()));
        TextComps = InTextInfo;
    }
    void UpdateHoverState(double InMouseX, double InMouseY) { IsHovered = IsInside(InMouseX, InMouseY); }
    void Clicked() {
        OnClick();
    }
    void Draw();
};
