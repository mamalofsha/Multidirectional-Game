#pragma once
#include "UIWindow.h"
#include "UIButton.h"
#include <map>

enum TabType
{
    tWorkShop,
    tDecoration
};

class UIPaginatedWindow : public UIWindow {
public:
    int currentPage = 0;
    int itemsPerPage = 5;
    std::vector<std::shared_ptr<UIButton>> pageControls; // Controls for navigation (next/previous page)
    std::map<std::string, std::vector<std::shared_ptr<UIElement>>> tabs;
    std::string xmlName;
    UIPaginatedWindow(float x, float y, float width, float height,std::string inXML)
        : UIWindow(x, y, width, height) {
        xmlName = inXML;
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

    template<typename T>
    inline void addTab(const std::string& UITabName, const std::string& xmlcategory) {
        // Ensure T is either WorkshopData or Decoration

        if constexpr (std::is_same<T, WorkshopData>::value) {
            // uiElement = std::make_shared<UIElement>(item.name, "Workshop");
           //  uiElement->setCost(item.cost);
           //  uiElement->setGoldGenerate(item.goldGenerate);
            std::vector<WorkshopData> items = XMLParser::LoadWorkShops(xmlName, xmlcategory);
            for (const auto& item : items) {
                std::cout << item.Name;

            }

        }
        else if constexpr (std::is_same<T, Decoration>::value) {
           // std::vector<Decoration> items = XMLParser::LoadItems<Decoration>(xmlName, xmlcategory);
            std::vector<Decoration> items = XMLParser::LoadDecorations(xmlName, xmlcategory);
            for (const auto& item : items) {
                std::cout << item.Name;
            }
            // uiElement = std::make_shared<UIElement>(item.name, "Decoration");
           //  uiElement->setCost(item.cost);
           //  uiElement->setHappiness(item.happiness);
        }
        // Load items from the XML
        // Convert items to UIElements and store in the tabs map

        // Add to tabs map
       // tabs[UITabName] = uiElements;
    }
};