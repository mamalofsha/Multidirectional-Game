#pragma once
#include "UIWindow.h"
#include "MouseObject.h"
#include "XMLParser.h"
#include "HUD.h"
class UIButton;

enum TabType
{
	tWorkShop,
	tDecoration
};

class UIPaginatedWindow : public UIWindow {
public:
	// config for page item count
	int Columns = 2;
	int Rows = 2;
	float SlotSpaceX = 0.6f;
	// - since it goes down
	float SlotSpaceY = -0.5f;
	int CurrentPage = 0;
	std::vector<std::shared_ptr<UIButton>> PageControls; // Controls next/previous page
	std::map<std::string, std::vector<std::shared_ptr<UIButton>>> Tabs;
	std::string XMLName;
	std::string ActiveTab = "Decorations";
	HUD* HudPtr;
	UIPaginatedWindow(std::shared_ptr<Shader> InShaderProgram, float InPosX, float InPosY, float InWidth, float InHeight, std::string InXML, const std::string& InAssetPath, HUD* InHud,std::string InActiveTab)
		: UIWindow(InShaderProgram, InPosX, InPosY, InWidth, InHeight) , ActiveTab(InActiveTab) {
		XMLName = InXML;
		HudPtr = InHud;
		InitializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{InPosX, InPosY}, std::vector<float>{InWidth, InHeight}, InAssetPath.c_str()));
	}
	 void UpdateChildrenButtons(float InX, float InY)override;
	void Draw() override;
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
			std::vector<UIText> Texts;
			UIText OutText = {50.0f,110.0f,Item.Name,0.4f };
			Texts.push_back(OutText);
			OutText = { 60.0f,135.0f,"$ " + std::to_string(static_cast<int>(Item.GoldCost)),0.4f};
			Texts.push_back(OutText);
			OutText = { 60.0f,160.0f,"Generates " + std::to_string(static_cast<int>(Item.GoldGenRate))+"$ per frame",0.4f};
			Texts.push_back(OutText);
			int X = UIElements.size() % Columns;
			int Y = std::floor((UIElements.size() / Columns) % Rows);
			std::shared_ptr<UIButton> Button = std::make_shared<UIButton>(HudPtr->GetUIShader(), -0.3f + (X * SlotSpaceX), 0.4f + (Y * SlotSpaceY), 0.25f, 0.25f, [&, Item]() {
				this->SetHidden(true);
				HudPtr->GetMouseObjectPtr()->SetItemID(Item.ItemID);
				HudPtr->GetMouseObjectPtr()->ReloadTexture(Item.ImageFile.c_str());
				HudPtr->ChangeExtraLayerActivity(false);
				},Item.ImageFile, HudPtr, Texts);
			UIElements.push_back(Button);
		}
	}
	else if constexpr (std::is_same<T, DecorationData>::value) {
		std::vector<DecorationData> Items = XMLParser::LoadDecorations(XMLName, InXMLCategory);
		for (const auto& Item : Items) {
			std::vector<UIText> Texts;
			UIText OutText = { 50.0f,110.0f,Item.Name,0.4f };
			Texts.push_back(OutText);
			OutText = { 60.0f,135.0f,"$ " + std::to_string(static_cast<int>(Item.GoldCost)),0.4f };
			Texts.push_back(OutText);
			OutText = { 60.0f,160.0f,"Generates " + std::to_string(static_cast<int>(Item.Happiness)) + " + per frame",0.4f };
			Texts.push_back(OutText);
			int X = UIElements.size() % Columns;
			int Y = std::floor((UIElements.size() / Columns) % Rows);
			std::shared_ptr<UIButton> Button = std::make_shared<UIButton>(HudPtr->GetUIShader(), -0.3f + (X * SlotSpaceX), 0.4f + (Y * SlotSpaceY), 0.2f, 0.2f, [&, Item]() {
				this->SetHidden(true);
				HudPtr->GetMouseObjectPtr()->SetItemID(Item.ItemID);
				HudPtr->GetMouseObjectPtr()->ReloadTexture(Item.ImageFile.c_str());
				HudPtr->ChangeExtraLayerActivity(false);
				},Item.ImageFile, HudPtr, Texts);
			UIElements.push_back(Button);
		}
	}
	Tabs[InUITabName] = UIElements;
}
