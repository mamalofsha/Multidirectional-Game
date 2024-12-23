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
struct Decoration;

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
    static std::vector<WorkshopData> LoadWorkShops(const std::string& InFileName, const std::string& InCategoryName);
	static std::vector<Decoration> LoadDecorations(const std::string& InFileName, const std::string& InCategoryName);
	static void UpdateGridValue(const std::string& filename, int gridX, int gridY, int newValue);
	void ResetSave(const std::string& filename);
	static int GetGridValue(const std::string& filename, int gridX, int gridY);
};


