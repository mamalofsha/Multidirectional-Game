#pragma once
#include "UIElement.h"
#include "HUD.h"
#include "World.h"
void UIElement::DrawText()
{
	float TextBaseScaleFactor = 0.8f;
	float TextBaseWindowFactorX = 2000.0f;
	float TextBaseWindowFactorY = 1404.0f;
	for (auto It = TextComps.begin(); It < TextComps.end(); It++)
	{
		float ScreenX = (PosX + 1.0f) * 0.5f * HudPtr->GetWindowSize().first;
		float ScreenY = (1.0f - PosY) * 0.5f * HudPtr->GetWindowSize().second; // Inversion for Y
		// this adjusts the text for different window scale    / TextBaseScaleFactor * HudPtr->GetWorldPtr()->GetStartupData().WindowScale
		// this adjusts the text for different window size  / TextBaseWindowFactor * HudPtr->GetWorldPtr()->GetStartupData().LevelWidth
		Graphics::RenderText(HudPtr->GetFontShader(), HudPtr->GetFontData().first, HudPtr->GetFontData().second,
			It->ActualText, (ScreenX - ((It->OffSetX / TextBaseScaleFactor * HudPtr->GetWorldPtr()->GetStartupData().WindowScale) / TextBaseWindowFactorX * HudPtr->GetWorldPtr()->GetStartupData().LevelWidth)),
			HudPtr->GetWindowSize().second - ScreenY - ((It->OffSetY / TextBaseScaleFactor * HudPtr->GetWorldPtr()->GetStartupData().WindowScale) / TextBaseWindowFactorY * HudPtr->GetWorldPtr()->GetStartupData().LevelHeight),
			(It->Scale / TextBaseScaleFactor * HudPtr->GetWorldPtr()->GetStartupData().WindowScale) / TextBaseWindowFactorX * (HudPtr->GetWorldPtr()->GetStartupData().LevelWidth),
			glm::vec3(0.0, 0.0f, 0.0f), HudPtr->GetCharacters());
	}
}

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
