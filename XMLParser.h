#pragma once
#include <string>
#include "Graphics.h"
#include <TinyXML/tinyxml2.h>
using namespace tinyxml2;

struct StartUpData
{
	int LevelWidth;
	int LevelHeight;
	float WindowScale;
	std::string GridFileName;
};

struct WorkshopData;
struct DecorationData;

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
    static std::vector<WorkshopData> LoadWorkShops(const std::string& InFileName, const std::string& InCategoryName);
	static WorkshopData LoadWorkShop(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName);
	static DecorationData LoadDecoration(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName);
	static std::vector<DecorationData> LoadDecorations(const std::string& InFileName, const std::string& InCategoryName);
	static void UpdateGridValue(const std::string& filename, int gridX, int gridY, const char* newValue);
	static void ResetSave(const std::string& filename);
	static std::string GetGridValue(const std::string& filename, int gridX, int gridY);
};


