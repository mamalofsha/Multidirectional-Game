#pragma once
#include <string>
#include "Graphics.h"
#include <TinyXML/tinyxml2.h>

struct StartUpData
{
	int LevelWidth;
	int LevelHeight;
	float WindowScale;
	std::string GridFileName;
};

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

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
    static std::vector<WorkshopData> LoadWorkShops(const std::string& InFileName, const std::string& InCategoryName);
	static std::vector<Decoration> LoadDecorations(const std::string& InFileName, const std::string& InCategoryName);

};


