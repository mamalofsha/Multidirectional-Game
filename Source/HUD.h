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
	unsigned int fontVAO, fontVBO;
	float WindowWidth, WindowHeight;
	World* Worldptr;
	Shader FontShader;
	std::map<GLchar, Character> Characters;
	std::shared_ptr<Shader> UIShader;

	std::vector<std::shared_ptr<class UIElement>> UIElements;
public:
	std::shared_ptr<class UIPaginatedWindow> shopWindow;
	std::shared_ptr<class MouseObject> mous;

	HUD(float inWindowWidth, float inWindowHeight, World* InWorld);
	void Update();
	void onHoverFunction(int gridX, int gridY, float screenX, float screenY);
	void onClickFunction(int gridX, int gridY, float screenX, float screenY);
	std::pair<unsigned int, unsigned int > GetFontData() { return{ fontVAO ,fontVBO }; };
	std::pair< float, float > GetWindowSize() { return{ WindowWidth ,WindowHeight }; };
	World* GetWorldPtr() { return Worldptr; };
	Shader& GetFontShader() { return FontShader; };
	std::shared_ptr<Shader> GetUIShader() { return UIShader; };
	std::map<GLchar, Character>& GetCharacters() { return Characters; };

};