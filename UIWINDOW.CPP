#include "Source/UIWindow.h"

void UIWindow::Draw()
{
    if (IsHidden) return;
    ObjectShader->use();
    for (auto& child : Children) {
        child->Draw();
    }
}

void UIWindow::Update(float mouseX, float mouseY, bool isClicked)
{
    if (IsHidden) return;
    for (auto& Child : Children) {
        Child->Update(mouseX, mouseY, isClicked);
    }
}

void UIWindow::SetHidden(bool newHidden)
{
    IsHidden = newHidden;
    for (auto& Child : Children) {
        Child->SetHidden(newHidden);
    }
}
