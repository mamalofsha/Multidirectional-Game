#pragma once
#include "Graphics.h"

struct ShopItemData
{
	std::string ItemID;
	std::string Name;
	std::string ImageFile;
	float GoldCost;
};

struct WorkshopData : public ShopItemData
{
	float GoldGenRate;
};
struct DecorationData :public ShopItemData
{
	float Happiness;
};

class World;
struct Character;
class HUD
{
private:
    unsigned int FontVAO, FontVBO;
    float WindowWidth, WindowHeight;
    World* WorldPtr;
    Shader FontShader;
    bool ExtraLayerActive = false;
    std::map<GLchar, Character> Characters;
    std::shared_ptr<Shader> UIShader;
    std::shared_ptr<class MouseObject> MouseObjectPtr;
    std::vector<std::shared_ptr<class UIElement>> UIElements;
public:
    std::shared_ptr<class UIPaginatedWindow> ShopWindow;
    std::shared_ptr<class UIDetailsWindow> ItemInfoPopUP;

    HUD(float InWindowWidth, float InWindowHeight, World* InWorld);
    void Update();
    void OnHoverFunction(int InGridX, int InGridY, float InScreenX, float InScreenY);
    void OnClickFunction(int InGridX, int InGridY, float InScreenX, float InScreenY);
    std::pair<unsigned int, unsigned int> GetFontData() { return { FontVAO, FontVBO }; };
    std::pair<float, float> GetWindowSize() { return { WindowWidth, WindowHeight }; };
    World* GetWorldPtr() { return WorldPtr; };
    Shader& GetFontShader() { return FontShader; };
    std::shared_ptr<Shader> GetUIShader() { return UIShader; };
    std::map<GLchar, Character>& GetCharacters() { return Characters; };
    std::shared_ptr<class MouseObject> GetMouseObjectPtr() { return MouseObjectPtr; };
    void ChangeExtraLayerActivity(bool InActivity) { ExtraLayerActive = InActivity; };
};