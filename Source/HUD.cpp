#include "HUD.h"
#include "UIPaginatedWindow.h"
#include "XMLParser.h"
#include <map>
#include "UIElement.h"
#include"World.h"
#include "MouseObject.h"
#include "UIButton.h"
#include "UIDetailsWindow.h"

HUD::HUD(float InWindowWidth, float InWindowHeight, World* InWorldPtr)
{
	// For having transparent images and also text
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	WindowWidth = InWindowWidth;
	WindowHeight = InWindowHeight;
	WorldPtr = InWorldPtr;
	FontShader = Graphics::InitTextRender(Characters, InWindowWidth, InWindowHeight, FontVAO, FontVBO);
	UIShader = std::make_shared<Shader>("Source/Shaders/UI.vert", "Source/Shaders/UI.frag");
	// loading up windows and ui stuff 
	// Vertex data with texture coordinates
	std::vector<float> Vertices = {
		// Positions      // Texture Coords
		-0.5f, -0.5f,     0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f,     1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f,     1.0f, 1.0f, // Top-right
		-0.5f,  0.5f,     0.0f, 1.0f  // Top-left
	};

	std::vector<unsigned int> Indices = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};
	VertexAttribute OutVertexData = { 4,{2, 2} };
	MouseObjectPtr = std::make_shared<MouseObject>(WorldPtr->BuildingShader, Vertices, Indices, "Assets/Images/bridge.png", OutVertexData, WorldPtr);
	MouseObjectPtr->SetSize(0.05f);
	///
	ShopWindow = std::make_shared<UIPaginatedWindow>(UIShader, 0.0f, 0.0f, 1.5f, 1.5f, "ShopItems.xml", "Assets/Images/grass.png", this, "Decorations");
	std::shared_ptr<UIButton> NextButton = std::make_shared<UIButton>(UIShader, 0.6f, -0.64f, 0.15f, 0.15f, [&]() {
		ShopWindow->NextPage();
		}, "Assets/Images/next.png", this);

	std::shared_ptr<UIButton> PrevButton = std::make_shared<UIButton>(UIShader, -0.6f, -0.64f, 0.15f, 0.15f, [&]() {
		ShopWindow->PreviousPage();
		}, "Assets/Images/previous.png", this);

	UIText TextData;
	TextData.ActualText = "Work Shops";
	TextData.OffSetX = 100.0f;
	TextData.OffSetY = 10.0f;
	TextData.Scale = 0.7f;
	std::vector<UIText> OutTextData;
	OutTextData.push_back(TextData);
	std::shared_ptr<UIButton> Cat1Button = std::make_shared<UIButton>(UIShader, -0.55f, 0.65f, 0.3f, 0.1f, [&]() {
		if (ShopWindow->ActiveTab == "Work Shops") return;
		ShopWindow->CurrentPage = 0;
		ShopWindow->ActiveTab = "Work Shops";
		}, "Assets/Images/buttonBG.png", this, OutTextData);
	OutTextData.clear();
	TextData.ActualText = "Decorations";
	TextData.OffSetX = 110.0f;
	TextData.OffSetY = 10.0f;
	TextData.Scale = 0.7f;
	OutTextData.push_back(TextData);
	std::shared_ptr<UIButton> Cat2Button = std::make_shared<UIButton>(UIShader, -0.05f, 0.65f, 0.3f, 0.1f, [&]() {
		if (ShopWindow->ActiveTab == "Decorations") return;
		ShopWindow->CurrentPage = 0;
		ShopWindow->ActiveTab = "Decorations";
		}, "Assets/Images/buttonBG.png", this, OutTextData);
	// so referencing it doesn't change it's ref counter
	std::weak_ptr<UIElement> WeakShopWindow = ShopWindow;
	std::shared_ptr<UIButton> ShopClose = std::make_shared<UIButton>(UIShader, 0.65f, 0.65f, 0.15f, 0.15f, [WeakShopWindow, this]() {
		if (auto SharedShopWindow = WeakShopWindow.lock()) {
			if(!SharedShopWindow->GetHidden()){
			SharedShopWindow->SetHidden(!SharedShopWindow->GetHidden());
			ExtraLayerActive = false;}
		}
		}, "Assets/Images/close.png", this);
	ShopWindow->AddTab<WorkshopData>("Work Shops", "workshops");
	ShopWindow->AddTab<DecorationData>("Decorations", "decorations");
	ShopWindow->PageControls.push_back(NextButton);
	ShopWindow->PageControls.push_back(PrevButton);
	ShopWindow->PageControls.push_back(Cat1Button);
	ShopWindow->PageControls.push_back(Cat2Button);
	ShopWindow->PageControls.push_back(ShopClose);
	ShopWindow->SetHidden(true);
	///
	std::shared_ptr<UIButton> ShopButton = std::make_shared<UIButton>(UIShader, 0.80f, -0.82f, .3f, .3f, [WeakShopWindow, this]() {
		std::cout << "Shop button clicked! Opening Shop UI..." << std::endl;
		if (auto SharedShopWindow = WeakShopWindow.lock()) {
			std::cout << "Toggling Shop Button visibility!" << std::endl;
			if (!ExtraLayerActive) {
				SharedShopWindow->SetHidden(!SharedShopWindow->GetHidden());
				ExtraLayerActive = !SharedShopWindow->GetHidden();
			}
		}
		}, "Assets/Images/shop.png", this);
	UIElements.push_back(ShopButton);
	UIElements.push_back(ShopWindow);
	///
	OutTextData.clear();
	TextData.ActualText = "Close";
	TextData.OffSetX = 0.0f;
	TextData.OffSetY = 0.0f;
	TextData.Scale = 1.0f;
	OutTextData.push_back(TextData);
	ItemInfoPopUP = std::make_shared<UIDetailsWindow>(UIShader, 0.0f, 0.0f, 1.5f, 1.5f, "Assets/Images/grass.png", this);
	std::shared_ptr<UIButton> DetailsClose = std::make_shared<UIButton>(UIShader, 0.7f, 0.7f, 0.2f, 0.1f, [&]() {
		ItemInfoPopUP->SetHidden(true);
		ExtraLayerActive = false;
		}, "Assets/Images/buttonBG.png", this, OutTextData);
	OutTextData.clear();
	TextData.ActualText = "Delete";
	TextData.OffSetX = 0.0f;
	TextData.OffSetY = 0.0f;
	TextData.Scale = 1.0f;
	OutTextData.push_back(TextData);
	std::shared_ptr<UIButton> DeletePage = std::make_shared<UIButton>(UIShader, -0.7f, 0.7f, 0.2f, 0.1f, [&]() {
		ItemInfoPopUP->ItemDelete();
		ItemInfoPopUP->SetHidden(true);
		ExtraLayerActive = false;

		}, "Assets/Images/buttonBG.png", this, OutTextData);
	UIText OutText = { 500.0f ,0.0f , "hello wow " , 1.0f , glm::vec3(0, 0, 0) };
	ItemInfoPopUP->AddText(OutText);
	ItemInfoPopUP->PageControls.push_back(DetailsClose);
	ItemInfoPopUP->PageControls.push_back(DeletePage);
	UIElements.push_back(ItemInfoPopUP);
	ItemInfoPopUP->SetHidden(true);
	//
	std::shared_ptr<UIButton> ZoomOut = std::make_shared<UIButton>(UIShader, 0.90f, 0.7f, .1f, .12f, [this]() {
		std::cout << "Shop button clicked! Opening Shop UI..." << std::endl;
		if (GetWorldPtr())
		{
			std::cout << " zooom outttt!" << std::endl;
			GetWorldPtr()->ChangeMagnifierZoom(-1);
		}
		}, "Assets/Images/zoomout.png", this);
	UIElements.push_back(ZoomOut);

	std::shared_ptr<UIButton> ZoomIn = std::make_shared<UIButton>(UIShader, 0.90f, 0.9f, .1f, .12f, [this]() {
		std::cout << "Shop button clicked! Opening Shop UI..." << std::endl;
		if (GetWorldPtr())
		{
			std::cout << " zooom outttt!" << std::endl;
			GetWorldPtr()->ChangeMagnifierZoom(1);
		}
		}, "Assets/Images/zoomin.png", this);
	UIElements.push_back(ZoomIn);
}

void HUD::Update()
{
	MouseObjectPtr->Draw();
	for (auto& Element : UIElements)
	{
		Element->Draw();
	}
	//ShopWindow->Draw();
	//ItemInfoPopUP->Draw();
}

void HUD::OnHoverFunction(int InGridX, int InGridY, float InScreenX, float InScreenY)
{
	std::cout << "Hovered over tile: (" << InGridX << ", " << InGridY << ")" << std::endl;
	if (UIElements.empty()) return;
	for (auto& Element : UIElements)
	{
		if (auto Button = std::dynamic_pointer_cast<UIButton>(Element)) {
			Button->UpdateHoverState(InScreenX, InScreenY);
		}
		if (auto Window = std::dynamic_pointer_cast<UIWindow>(Element)) {
			Window->UpdateChildrenButtons(InScreenX, InScreenY);
		}
	}
}

void HUD::OnClickFunction(int InGridX, int InGridY, float InScreenX, float InScreenY)
{
	if (!MouseObjectPtr->GetHidden())
	{
		MouseObjectPtr->ExecuteAction();
		std::cout << "Mouse object action executed!" << std::endl;
	}
	else
	{
		if (!ExtraLayerActive)
		{
			// clicking on item popup 
			std::string GridValue = XMLParser::GetGridValue(WorldPtr->GetStartupData().GridFileName, WorldPtr->GetMouseState().GridX, WorldPtr->GetMouseState().GridY);
			if (GridValue != "0")
			{
				WorkshopData TempWorkShopData = XMLParser::LoadWorkshop("ShopItems.xml", "workshops", GridValue);
				if (!TempWorkShopData.Name.empty())
				{
					std::vector<UIText> OutTextData;
					UIText Name = { 600.0f,300.0f,"Name: " + TempWorkShopData.Name,1.0f,glm::vec3(0,0,0) };
					OutTextData.push_back(Name);
					UIText Gold = { 600.0f,100.0f,"Gold Gen: " + std::to_string(TempWorkShopData.GoldGenRate),1.0f,glm::vec3(0,0,0) };
					OutTextData.push_back(Gold);
					ExtraLayerActive = true;
					ItemInfoPopUP->Override(OutTextData, TempWorkShopData.ImageFile.c_str(), WorldPtr->GetMouseState().GridX, WorldPtr->GetMouseState().GridY);
				}
				else
				{
					DecorationData TempDecorationData = XMLParser::LoadDecoration("ShopItems.xml", "decorations", GridValue);
					if (!TempDecorationData.Name.empty())
					{
						std::vector<UIText> OutTextData;
						UIText Name = { 600.0f,300.0f,"Name: " + TempDecorationData.Name,1.0f,glm::vec3(0,0,0) };
						OutTextData.push_back(Name);
						UIText Happy = { 600.0f,100.0f,"Happiness : " + std::to_string(TempDecorationData.Happiness),1.0f,glm::vec3(0,0,0) };
						OutTextData.push_back(Happy);
						ItemInfoPopUP->Override(OutTextData, TempDecorationData.ImageFile.c_str(), WorldPtr->GetMouseState().GridX, WorldPtr->GetMouseState().GridY);
						ExtraLayerActive = true;
					}
				}
			}
		}
	}
	for (auto& Element : UIElements)
	{
		if (auto Button = std::dynamic_pointer_cast<UIButton>(Element)) {
			if (Button->IsHovered) {
				Button->Clicked();
				Button->OnClick();
			}
		}
		if (auto Window = std::dynamic_pointer_cast<UIPaginatedWindow>(Element)) {
			for (auto& Control : Window->PageControls)
			{
				if (Control->IsHovered) {
					Control->Clicked();
					Control->OnClick();
				}
			}
			for (auto& Button : Window->GetCatButtons())
			{
				if (Button->IsHovered) {
					Button->Clicked();
					Button->OnClick();
				}
			}
		}
		if (auto Window = std::dynamic_pointer_cast<UIDetailsWindow>(Element)) {
			for (auto& Control : Window->PageControls)
			{
				if (Control->IsHovered) {
					Control->Clicked();
					Control->OnClick();
				}
			}
		}
	}
}