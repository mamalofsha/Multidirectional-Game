#include "HUD.h"
#include "UIPaginatedWindow.h"
#include "XMLParser.h"
#include <map>
#include "UIElement.h"
HUD::HUD(float inWindowWidth, float inWindowHeight)
{
	WindowWidth = inWindowWidth;
	WindowHeight = inWindowHeight;
	font = Graphics::InitTextRender(Characters, inWindowWidth,inWindowHeight,fontVAO,fontVBO);
	UIE = std::make_shared<Shader>("UI.vert", "UI.frag");
	
	/// callmuse
	shopWindow = std::make_shared<UIPaginatedWindow>(UIE,0.0f, 0.0f, 1.5f, 1.5f, "ShopItems.xml","grass.png", this);
	//Graphics::DrawUIElement(UIE ,*shopWindow, "grass.png");
	
	std::shared_ptr<UIButton> nextButton = std::make_shared<UIButton>(UIE,0.6f, -0.7f, 0.2f, 0.1f, [&]() {
		shopWindow->nextPage();

		},"", "shop.png", this);
	//Graphics::DrawUIElement(UIE, *nextButton, "shop.png");

	std::shared_ptr<UIButton> prevButton = std::make_shared<UIButton>(UIE ,-0.6f, -0.7f, 0.2f, 0.1f, [&]() {
		shopWindow->previousPage();
		}, "", "shop.png", this);
	//Graphics::DrawUIElement(UIE, *prevButton, "shop.png");

	std::shared_ptr<UIButton> cat1 = std::make_shared<UIButton>(UIE ,-0.6f, 0.7f, 0.2f, 0.1f, [&]() {
		if (shopWindow->ActiveTab == "Work Shops") return;
		shopWindow->currentPage = 0;
		shopWindow->ActiveTab = "Work Shops";
		},"wowo", "buttonBG.png", this);
	std::shared_ptr<UIButton> cat2 = std::make_shared<UIButton>(UIE ,-0.f, 0.7f, 0.2f, 0.1f, [&]() {
		if (shopWindow->ActiveTab == "Decorations") return;
		shopWindow->currentPage = 0;
		shopWindow->ActiveTab = "Decorations";
		},"Dec", "buttonBG.png", this);
	shopWindow->addTab<WorkshopData>("Work Shops", "workshops");
	shopWindow->addTab<Decoration>("Decorations", "decorations");
	shopWindow->pageControls.push_back(nextButton);
	shopWindow->pageControls.push_back(prevButton);
	shopWindow->pageControls.push_back(cat1);
	shopWindow->pageControls.push_back(cat2);

	shopWindow->SetHidden(true);
	

	std::weak_ptr<UIElement> weakSdd = shopWindow; // Create a weak pointer

	std::shared_ptr<UIButton> sd = std::make_shared<UIButton>(UIE,0.80f, -0.82f, .3f, .3f, [weakSdd]() {
		std::cout << "Shop button clicked! Opening Shop UI..." << std::endl;
		if (auto sharedSdd = weakSdd.lock()) { // Check if the weak pointer is still valid
			std::cout << "Toggling Shop Button visibility!" << std::endl;
			sharedSdd->SetHidden(!sharedSdd->isHidden);
		}
		else {
			//std::cerr << "Shop button is no longer valid!" << std::endl;
		}
		}, "", "shop.png", this);
	UIElements.push_back(sd);
	UIElements.push_back(shopWindow);
}

void HUD::Update()
{
	//UIE->use();
	for (auto& Element : UIElements )
	{
		Element->draw();
	}
shopWindow->draw();
}

void HUD::onHoverFunction(int gridX, int gridY, float screenX, float screenY)
{
	std::cout << "Hovereddead over tile: (" << gridX << ", " << gridY << ")" << std::endl;
	if (UIElements.empty()) return;
	for (auto& element : UIElements)
	{
		if (auto button = std::dynamic_pointer_cast<UIButton>(element)) {
			// Successfully cast, so the object is a Button
			button->updateHoverState(screenX, screenY);
		}
		if (auto Window = std::dynamic_pointer_cast<UIPaginatedWindow>(element)) {
			// Successfully cast, so the object is a Button
			Window->UpdateChildrenButtons(screenX, screenY);
		}
	}
	//mouseState.GridX = gridX;
	//mouseState.GridY = gridY;
}

void HUD::onClickFunction(int gridX, int gridY, float screenX, float screenY)
{
	for (auto& element : UIElements)
	{
		if (auto button = std::dynamic_pointer_cast<UIButton>(element)) {
			if (button->isHovered) {
				button->cllicked();
				button->onClick();
			}
		}
		if (auto Window = std::dynamic_pointer_cast<UIPaginatedWindow>(element)) {
			// Successfully cast, so the object is a Button
			for (auto& el : Window->pageControls)
			{
					if (el->isHovered) {
						el->cllicked();
						el->onClick();
					}
				
			}
			for (auto& el : Window->getCatButtons())
			{
				if (el->isHovered) {
					el->cllicked();
					el->onClick();
				}

			}
		}
	}
}
