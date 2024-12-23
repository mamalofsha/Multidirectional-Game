#include "Source/UIWindow.h"

void UIWindow::Draw()
{
    if (IsHidden) return;
    ObjectShader->use();
    for (auto& Child : Children) {
        Child->Draw();
    }
}

void UIWindow::Update(float InMouseX, float InMouseY, bool IsClicked)
{
    if (IsHidden) return;
    for (auto& Child : Children) {
        Child->Update(InMouseX, InMouseY, IsClicked);
    }
}

void UIWindow::SetHidden(bool NewHidden)
{
    IsHidden = NewHidden;
    for (auto& Child : Children) {
        Child->SetHidden(NewHidden);
    }
}
