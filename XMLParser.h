#pragma once
#include <string>
#include "Graphics.h"

struct StartUpData
{
	int LevelWidth;
	int LevelHeight;
	float WindowScale;
	std::string GridFileName;
};

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
};