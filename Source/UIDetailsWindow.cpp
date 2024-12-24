#include "UIDetailsWindow.h"
#include "UIButton.h"
#include "HUD.h"
#include "XMLParser.h"
#include "World.h"
void UIDetailsWindow::Draw()
{
    if (IsHidden) return;
    ObjectShader->use();
    ObjectShader->setBool("IsHidden", IsHidden);
    ObjectShader->setBool("IsHovered", false);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (auto It = PageControls.begin(); It != PageControls.end(); ++It) {
        (*It)->Draw(); 
    }
    for (auto It = TextComps.begin(); It != TextComps.end(); ++It) {
        
        float ScreenX = (PosX + 1.0f) * 0.5f * HudPtr->GetWindowSize().first;
        float ScreenY = (1.0f - PosY) * 0.5f * HudPtr->GetWindowSize().second; // Inversion for Y
        Graphics::RenderText(HudPtr->GetFontShader(), HudPtr->GetFontData().first, HudPtr->GetFontData().second,
            (*It).ActualText, ScreenX - (*It).OffSetX, HudPtr->GetWindowSize().second - ScreenY - (*It).OffSetY, (*It).Scale,
            (*It).TextColor, HudPtr->GetCharacters());
    }
}

void UIDetailsWindow::AddText(UIText InText)
{
    TextComps.push_back(InText);
}

void UIDetailsWindow::Override(std::vector<UIText> InText, const char* InFilePath, int InGridX, int InGridY)
{
    TextComps.clear();
    TextComps = InText;
    SetHidden(false);
    GridX = InGridX;
    GridY = InGridY;
}

void UIDetailsWindow::ItemDelete()
{
     XMLParser::UpdateGridValue(HudPtr->GetWorldPtr()->GetStartupData().GridFileName,GridX,GridY,"0");
     HudPtr->GetWorldPtr()->DeleteBuilding(GridX, GridY);
}

void UIDetailsWindow::UpdateChildrenButtons(float InX, float InY)
{
    for (auto& Child : PageControls)
    {
        if (Child)
        {
            if (IsHidden)
            {
                Child->IsHovered = false;
                continue;
            }
            Child->UpdateHoverState(InX, InY);
        }
    }
}
