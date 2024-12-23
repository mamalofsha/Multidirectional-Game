#pragma once
#include "UIElement.h"

bool UIElement::IsInside(float InMouseX, float InMouseY) const
{
    float HalfWidth = Width / 2.0f;
    float HalfHeight = Height / 2.0f;
    return InMouseX >= PosX - HalfWidth && InMouseX <= PosX + HalfWidth &&
        InMouseY <= PosY + HalfHeight && InMouseY >= PosY - HalfHeight;
}

void UIElement::InitializeFromRenderData(const RenderData& InData)
{
    VAO = InData.VAO;
    VBO = InData.VBO;
    EBO = InData.EBO;
    Texture = InData.TextureID;
}
