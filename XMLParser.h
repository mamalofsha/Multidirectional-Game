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

struct WorkshopData;
struct Decoration;

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
    static std::vector<WorkshopData> LoadWorkShops(const std::string& InFileName, const std::string& InCategoryName);
	static std::vector<Decoration> LoadDecorations(const std::string& InFileName, const std::string& InCategoryName);

};


