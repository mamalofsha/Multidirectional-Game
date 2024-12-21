#pragma once
#include "Shader.h"
#include "Graphics.h"

struct ShopItemData
{
	std::string Name;
	std::string ImageFile;
	float goldCost;
};

struct WorkshopData : public ShopItemData
{
	float goldGenrate;
};
struct Decoration :public ShopItemData
{
	float happiness;
};



struct Character;
class HUD
{
private:

	std::vector<std::shared_ptr<class UIElement>> UIElements;
public:
	std::shared_ptr<class UIPaginatedWindow> shopWindow;

	std::map<GLchar, Character> Characters;
	Shader font;
	float WindowWidth, WindowHeight;
	HUD(float inWindowWidth, float inWindowHeight);
	void Update();
	void onHoverFunction(int gridX, int gridY, float screenX, float screenY);
	void onClickFunction(int gridX, int gridY, float screenX, float screenY);
};