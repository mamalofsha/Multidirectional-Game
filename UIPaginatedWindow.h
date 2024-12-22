#pragma once
#include "UIWindow.h"
#include "UIButton.h"
#include <map>
#include "XMLParser.h"

enum TabType
{
	tWorkShop,
	tDecoration
};

class UIPaginatedWindow : public UIWindow {
public:
	int columns = 2;
	int rows = 2;
	int currentPage = 0;
	std::vector<std::shared_ptr<UIButton>> pageControls; // Controls for navigation (next/previous page)
	std::map<std::string, std::vector<std::shared_ptr<UIButton>>> tabs;
	std::string xmlName;
	std::string ActiveTab = "Decorations";
	HUD* Hudptr;
	UIPaginatedWindow(std::shared_ptr<Shader> shaderProgram, float x, float y, float width, float height, std::string inXML, HUD* InHud)
		: UIWindow(shaderProgram, x, y, width, height) {
		xmlName = inXML;
		Hudptr = InHud;
		Graphics::DrawUIElement(shaderProgram, *this, "grass.png");
	}

	std::vector<std::shared_ptr<UIButton>> getCatButtons()
	{
		return tabs[ActiveTab];
	}

	void draw() override {
		if (isHidden) return;

		// Render the window background
		ObjectShader->use();
		ObjectShader->setBool("isHidden", isHidden);
		ObjectShader->setBool("isHovered", false);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		int startIndex = currentPage * rows * columns;

		int endIndex = startIndex + (rows * columns);


		for (auto it = pageControls.begin(); it != pageControls.end(); ++it) {
			(*it)->draw();// Access the UIButton via the dereferenced iterator
		}

		for (const auto& [tabName, buttons] : tabs) {
			//std::cout << "Tab: " << tabName << std::endl;
			if (tabName != ActiveTab) continue;
			for (int i = startIndex; i < endIndex; ++i) {
				if (i >= buttons.size())break;
				buttons[i]->draw();
			}
		}

	}

	void UpdateChildrenButtons(float x, float y)
	{
		
		for (auto& Child : pageControls)
		{
			if (Child)
			{
				if (isHidden)
				{
					Child->isHovered = false;
					continue;
				}
				Child->updateHoverState(x, y);
			}
		}
		int startIndex = currentPage * rows * columns;
		int endIndex;
		if (static_cast<int>((tabs[ActiveTab].size()) - startIndex >= (rows * columns)))
			endIndex = startIndex + (rows * columns);
		else
			endIndex = (tabs[ActiveTab].size());
		for (const auto& [tabName, buttons] : tabs) {
			//std::cout << "Tab: " << tabName << std::endl;
			if (tabName != ActiveTab) continue;
			for (int i = startIndex; i < endIndex; ++i) {
				if (isHidden)
				{
					buttons[i]->isHovered = false;
					continue;
				}
				buttons[i]->updateHoverState(x, y);
				if (buttons[i]->isHovered)
					std::cout << i << std::endl;
			}
		}
	}
	void addButton(std::shared_ptr<UIButton> child) {
		pageControls.push_back(child);
	}

	void nextPage() {
		if ((currentPage + 1) * (rows * columns) < tabs[ActiveTab].size()) {
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
		std::vector<std::shared_ptr<UIButton>> uiElements;

		if constexpr (std::is_same<T, WorkshopData>::value) {
			// uiElement = std::make_shared<UIElement>(item.name, "Workshop");
		   //  uiElement->setCost(item.cost);
		   //  uiElement->setGoldGenerate(item.goldGenerate);
			std::vector<WorkshopData> items = XMLParser::LoadWorkShops(xmlName, xmlcategory);
			for (const auto& item : items) {
				std::cout << item.Name;

				int x = uiElements.size() % columns;
				int y = std::floor((uiElements.size() / columns) % rows);

				int yhalf = y / columns;
				std::shared_ptr<UIButton> button = std::make_shared<UIButton>(Hudptr->UIE, -0.3f + (x * 0.6f), 0.4f + (y * -0.4f), 0.2f, 0.1f, [&, item]() {
					std::cout << "Spawned item: " << item.Name << " and attached to the mouse." << std::endl;
					}, item.Name, item.ImageFile, Hudptr);
				uiElements.push_back(button);

			}

		}
		else if constexpr (std::is_same<T, Decoration>::value) {
			// std::vector<Decoration> items = XMLParser::LoadItems<Decoration>(xmlName, xmlcategory);
			std::vector<Decoration> items = XMLParser::LoadDecorations(xmlName, xmlcategory);
			for (const auto& item : items) {
				std::cout << item.Name;

				int x = uiElements.size() % columns;
				int y = std::floor((uiElements.size() / columns) % rows);

				int yhalf = y / columns;
				std::shared_ptr<UIButton> button = std::make_shared<UIButton>(Hudptr->UIE, -0.3f + (x * 0.6f), 0.4f + (y * -0.4f), 0.2f, 0.1f, [&]() {
					this->SetHidden(true);
					}, item.Name, item.ImageFile, Hudptr);
				uiElements.push_back(button);
			}
			// uiElement = std::make_shared<UIElement>(item.name, "Decoration");
		   //  uiElement->setCost(item.cost);
		   //  uiElement->setHappiness(item.happiness);
		}
		// Load items from the XML
		// Convert items to UIElements and store in the tabs map

		// Add to tabs map
		tabs[UITabName] = uiElements;
	}
};