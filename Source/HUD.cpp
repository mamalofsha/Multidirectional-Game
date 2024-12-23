#include "HUD.h"
#include "UIPaginatedWindow.h"
#include "XMLParser.h"
#include <map>
#include "UIElement.h"
#include"World.h"
#include "MouseObject.h"
HUD::HUD(float inWindowWidth, float inWindowHeight,World* InWorld)
{
	// for having transparent images and also text
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	WindowWidth = inWindowWidth;
	WindowHeight = inWindowHeight;
	Worldptr = InWorld;
	FontShader = Graphics::InitTextRender(Characters, inWindowWidth,inWindowHeight,fontVAO,fontVBO);
	UIShader = std::make_shared<Shader>("Source/Shaders/UI.vert", "Source/Shaders/UI.frag");
	//
	// Vertex data with texture coordinates
	std::vector<float> vertices = {
		// Positions      // Texture Coords
		-0.5f, -0.5f,     0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f,     1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f,     1.0f, 1.0f, // Top-right
		-0.5f,  0.5f,     0.0f, 1.0f  // Top-left
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};
	VertexAttribute OutVertexData = { 4,{2,2} };
	mous = std::make_shared<MouseObject>(Worldptr->BuildingShader, vertices, indices, "Assets/Images/bridge.png", OutVertexData, Worldptr);
	mous->SetSize(0.05f);
	/// callmuse
	shopWindow = std::make_shared<UIPaginatedWindow>(UIShader,0.0f, 0.0f, 1.5f, 1.5f, "ShopItems.xml","Assets/Images/grass.png", this);
	//Graphics::DrawUIElement(UIE ,*shopWindow, "grass.png");
	
	std::shared_ptr<UIButton> nextButton = std::make_shared<UIButton>(UIShader,0.6f, -0.7f, 0.2f, 0.1f, [&]() {
		shopWindow->NextPage();

		},"", "Assets/Images/shop.png", this);
	//Graphics::DrawUIElement(UIE, *nextButton, "shop.png");

	std::shared_ptr<UIButton> prevButton = std::make_shared<UIButton>(UIShader,-0.6f, -0.7f, 0.2f, 0.1f, [&]() {
		shopWindow->PreviousPage();
		}, "", "Assets/Images/shop.png", this);
	//Graphics::DrawUIElement(UIE, *prevButton, "shop.png");

	std::shared_ptr<UIButton> cat1 = std::make_shared<UIButton>(UIShader,-0.6f, 0.7f, 0.2f, 0.1f, [&]() {
		if (shopWindow->ActiveTab == "Work Shops") return;
		shopWindow->CurrentPage = 0;
		shopWindow->ActiveTab = "Work Shops";
		},"wowo", "Assets/Images/buttonBG.png", this);
	std::shared_ptr<UIButton> cat2 = std::make_shared<UIButton>(UIShader,-0.f, 0.7f, 0.2f, 0.1f, [&]() {
		if (shopWindow->ActiveTab == "Decorations") return;
		shopWindow->CurrentPage = 0;
		shopWindow->ActiveTab = "Decorations";
		},"Dec", "Assets/Images/buttonBG.png", this);
	shopWindow->AddTab<WorkshopData>("Work Shops", "workshops");
	shopWindow->AddTab<DecorationData>("Decorations", "decorations");
	shopWindow->PageControls.push_back(nextButton);
	shopWindow->PageControls.push_back(prevButton);
	shopWindow->PageControls.push_back(cat1);
	shopWindow->PageControls.push_back(cat2);

	shopWindow->SetHidden(true);
	

	std::weak_ptr<UIElement> weakSdd = shopWindow; // Create a weak pointer

	std::shared_ptr<UIButton> sd = std::make_shared<UIButton>(UIShader,0.80f, -0.82f, .3f, .3f, [weakSdd]() {
		std::cout << "Shop button clicked! Opening Shop UI..." << std::endl;
		if (auto sharedSdd = weakSdd.lock()) { // Check if the weak pointer is still valid
			std::cout << "Toggling Shop Button visibility!" << std::endl;
			sharedSdd->SetHidden(!sharedSdd->GetHidden());
		}
		else {
			//std::cerr << "Shop button is no longer valid!" << std::endl;
		}
		}, "", "Assets/Images/shop.png", this);
	UIElements.push_back(sd);
	UIElements.push_back(shopWindow);
}

void HUD::Update()
{
	mous->Draw();
	//UIE->use();
	for (auto& Element : UIElements )
	{
		Element->Draw();
	}
shopWindow->Draw();
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
	if (!mous->GetHidden())
	{
		mous->ExecuteAction();
		std::cout << "decdi";
		//if (mous->GetIsAttachedtoGrid())
		//{
		//	mous->SetHidden(true);
		//}
	}
	for (auto& element : UIElements)
	{
		if (auto button = std::dynamic_pointer_cast<UIButton>(element)) {
			if (button->IsHovered) {
				button->cllicked();
				button->onClick();
			}
		}
		if (auto Window = std::dynamic_pointer_cast<UIPaginatedWindow>(element)) {
			// Successfully cast, so the object is a Button
			for (auto& el : Window->PageControls)
			{
					if (el->IsHovered) {
						el->cllicked();
						el->onClick();
					}
				
			}
			for (auto& el : Window->GetCatButtons())
			{
				if (el->IsHovered) {
					el->cllicked();
					el->onClick();
				}

			}
		}
	}

}
 