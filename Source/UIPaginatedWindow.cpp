#include "UIPaginatedWindow.h"
#include "UIButton.h"

void UIPaginatedWindow::Draw()
{
    if (IsHidden) return;

    // Render the window background
    ObjectShader->use();
    ObjectShader->setBool("IsHidden", IsHidden);
     ObjectShader->setBool("IsHovered", false);
     glBindTexture(GL_TEXTURE_2D, Texture);
     glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    int StartIndex = CurrentPage * Rows * Columns;
    int EndIndex = StartIndex + (Rows * Columns);

    for (auto It = PageControls.begin(); It != PageControls.end(); ++It) {
        (*It)->Draw(); // Access the UIButton via the dereferenced iterator
    }

    for (const auto& [TabName, Buttons] : Tabs) {
        if (TabName != ActiveTab) continue;
        for (int i = StartIndex; i < EndIndex; ++i) {
            if (i >= Buttons.size()) break;
            Buttons[i]->Draw();
        }
    }
}

void UIPaginatedWindow::UpdateChildrenButtons(float InX, float InY)
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

    int StartIndex = CurrentPage * Rows * Columns;
    int EndIndex;
    if (static_cast<int>((Tabs[ActiveTab].size()) - StartIndex >= (Rows * Columns)))
        EndIndex = StartIndex + (Rows * Columns);
    else
        EndIndex = (Tabs[ActiveTab].size());

    for (const auto& [TabName, Buttons] : Tabs) {
        if (TabName != ActiveTab) continue;
        for (int i = StartIndex; i < EndIndex; ++i) {
            if (IsHidden)
            {
                Buttons[i]->IsHovered = false;
                continue;
            }
            Buttons[i]->UpdateHoverState(InX, InY);
            if (Buttons[i]->IsHovered)
                std::cout << i << std::endl;
        }
    }
}

void UIPaginatedWindow::NextPage()
{
    if ((CurrentPage + 1) * (Rows * Columns) < Tabs[ActiveTab].size()) {
        CurrentPage++;
    }
}

void UIPaginatedWindow::PreviousPage()
{
    if ((CurrentPage - 1) >=  0 ) {
        CurrentPage--;
    }
}
