#pragma once
#include "UIWindow.h"
#include "UIButton.h"

class UIPaginatedWindow : public UIWindow {
public:
    int currentPage = 0;
    int itemsPerPage = 5;
    std::vector<std::shared_ptr<UIButton>> pageControls; // Controls for navigation (next/previous page)

    UIPaginatedWindow(float x, float y, float width, float height)
        : UIWindow(x, y, width, height) {
    }

    void draw() override {
        if (isHidden) return;

        // Render the window background
        ObjectShader->use();
        ObjectShader->setBool("isHidden", isHidden);

        glBindTexture(GL_TEXTURE_2D, ObjectShader->Texture);
        glBindVertexArray(ObjectShader->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        int startIndex = currentPage * itemsPerPage;
        int endIndex = std::min(static_cast<int>(children.size()), startIndex + itemsPerPage);

        for (int i = startIndex; i < endIndex; ++i) {
            children[i]->draw();
        }
        for (auto it = pageControls.begin(); it != pageControls.end(); ++it) {
            (*it)->draw();// Access the UIButton via the dereferenced iterator
        }

    }

    void UpdateChildrenButtons(float x, float y)
    {
        for (auto& Child : pageControls)
        {
            auto button = std::dynamic_pointer_cast<UIButton>(Child);
            if (button)
            {
                button->updateHoverState(x, y);
            }
        }
    }
    void addButton(std::shared_ptr<UIButton> child) {
        pageControls.push_back(child);
    }

    void nextPage() {
        if ((currentPage + 1) * itemsPerPage < children.size()) {
            currentPage++;
        }
    }

    void previousPage() {
        if (currentPage > 0) {
            currentPage--;
        }
    }
};