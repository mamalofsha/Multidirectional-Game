#pragma once
#include "UIElement.h"

class UIWindow : public UIElement {
public:
    std::vector<std::shared_ptr<UIElement>> Children;

    UIWindow(std::shared_ptr<Shader> InShaderProgram, float InPosX, float InPosY, float InWidth, float InHeight)
        : UIElement(InShaderProgram, InPosX, InPosY, InWidth, InHeight) {
    }
    void Draw() override;
    void Update(float InMouseX, float InMouseY, bool IsClicked) override;
    virtual void SetHidden(bool NewHidden)override;
};
