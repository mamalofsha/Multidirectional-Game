#pragma once
#include "UIButton.h"
#include "World.h"
void UIButton::Draw()
{
    auto SharedObjectShader = ObjectShader.lock();
    if (!SharedObjectShader) return;
    SharedObjectShader->use();
    SharedObjectShader->setBool("IsHovered", IsHovered);
    SharedObjectShader->setBool("IsHidden", IsHidden);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (HoverOnlyText && !IsHovered) return;
    if (TextComps.empty() || !HudPtr) return;
    DrawText();
}
