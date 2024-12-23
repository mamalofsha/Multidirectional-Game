#pragma once
#include "UIButton.h"

void UIButton::Draw()
{
    ObjectShader->use();
    ObjectShader->setBool("IsHovered", IsHovered);
    ObjectShader->setBool("IsHidden", IsHidden);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (Text.empty() || !HudPtr) return;
    float ScreenX = (PosX + 1.0f) * 0.5f * HudPtr->GetWindowSize().first;
    float ScreenY = (1.0f - PosY) * 0.5f * HudPtr->GetWindowSize().second; // Inversion for Y
    Graphics::RenderText(HudPtr->GetFontShader(), HudPtr->GetFontData().first, HudPtr->GetFontData().second,
        Text, ScreenX - 100.0f, HudPtr->GetWindowSize().second - ScreenY, 1.0f,
        glm::vec3(0.0, 0.0f, 0.0f), HudPtr->GetCharacters());
}
