#pragma once
#include "Shader.h"
#include "Graphics.h"

struct ShopItemData
{
	std::string ItemID;
	std::string Name;
	std::string ImageFile;
	float goldCost;
};

struct WorkshopData : public ShopItemData
{
	float goldGenrate;
};
struct DecorationData :public ShopItemData
{
	float happiness;
};


class World;
struct Character;
class HUD
{
private:

	std::vector<std::shared_ptr<class UIElement>> UIElements;
public:
	std::shared_ptr<class UIPaginatedWindow> shopWindow;
	std::shared_ptr<class MouseObject> mous;
	 World* Worldptr;
	std::map<GLchar, Character> Characters;
	Shader font;
	unsigned int fontVAO, fontVBO;
	std::shared_ptr<Shader> UIE;
	float WindowWidth, WindowHeight;
	HUD(float inWindowWidth, float inWindowHeight, World* InWorld);
	void Update();
	void onHoverFunction(int gridX, int gridY, float screenX, float screenY);
	void onClickFunction(int gridX, int gridY, float screenX, float screenY);
};