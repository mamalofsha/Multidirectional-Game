#pragma once
#include "UIWindow.h"
#include "UIButton.h"
#include <map>
#include "XMLParser.h"
#include "MouseObject.h"

enum TabType
{
	tWorkShop,
	tDecoration
};

class UIPaginatedWindow : public UIWindow {
public:
	int Columns = 2;
	int Rows = 2;
	int CurrentPage = 0;
	std::vector<std::shared_ptr<UIButton>> PageControls; // Controls for navigation (next/previous page)
	std::map<std::string, std::vector<std::shared_ptr<UIButton>>> Tabs;
	std::string XMLName;
	std::string ActiveTab = "Decorations";
	HUD* HudPtr;

	UIPaginatedWindow(std::shared_ptr<Shader> InShaderProgram, float InPosX, float InPosY, float InWidth, float InHeight, std::string InXML, const std::string& InAssetPath, HUD* InHud)
		: UIWindow(InShaderProgram, InPosX, InPosY, InWidth, InHeight) {
		XMLName = InXML;
		HudPtr = InHud;
		InitializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{InPosX, InPosY}, std::vector<float>{InWidth, InHeight}, InAssetPath.c_str()));
	}

	void Draw() override;
	void UpdateChildrenButtons(float InX, float InY);
	void AddButton(std::shared_ptr<UIButton> InChild) { PageControls.push_back(InChild); };
	void NextPage();
	void PreviousPage();
	template<typename T>
	inline void AddTab(const std::string& InUITabName, const std::string& InXMLCategory);
	std::vector<std::shared_ptr<UIButton>> GetCatButtons() { return Tabs[ActiveTab]; };
};

template<typename T>
inline void UIPaginatedWindow::AddTab(const std::string& InUITabName, const std::string& InXMLCategory)
{

	std::vector<std::shared_ptr<UIButton>> UIElements;

	if constexpr (std::is_same<T, WorkshopData>::value) {
		std::vector<WorkshopData> Items = XMLParser::LoadWorkshops(XMLName, InXMLCategory);
		for (const auto& Item : Items) {
			std::cout << Item.Name;

			int X = UIElements.size() % Columns;
			int Y = std::floor((UIElements.size() / Columns) % Rows);

			std::shared_ptr<UIButton> Button = std::make_shared<UIButton>(HudPtr->GetUIShader(), -0.3f + (X * 0.6f), 0.4f + (Y * -0.4f), 0.2f, 0.1f, [&, Item]() {
				std::cout << "Spawned item: " << Item.Name << " and attached to the mouse." << std::endl;
				this->SetHidden(true);
				HudPtr->mous->SetItemID(Item.ItemID);
				HudPtr->mous->ReloadTexture(Item.ImageFile.c_str());
				}, Item.Name, Item.ImageFile, HudPtr);
			UIElements.push_back(Button);

		}

	}
	else if constexpr (std::is_same<T, DecorationData>::value) {
		std::vector<DecorationData> Items = XMLParser::LoadDecorations(XMLName, InXMLCategory);
		for (const auto& Item : Items) {
			std::cout << Item.Name;

			int X = UIElements.size() % Columns;
			int Y = std::floor((UIElements.size() / Columns) % Rows);

			std::shared_ptr<UIButton> Button = std::make_shared<UIButton>(HudPtr->GetUIShader(), -0.3f + (X * 0.6f), 0.4f + (Y * -0.4f), 0.2f, 0.1f, [&, Item]() {
				this->SetHidden(true);
				HudPtr->mous->SetItemID(Item.ItemID);
				HudPtr->mous->ReloadTexture(Item.ImageFile.c_str());
				}, Item.Name, Item.ImageFile, HudPtr);
			UIElements.push_back(Button);
		}
	}

	Tabs[InUITabName] = UIElements;

}
