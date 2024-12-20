#pragma once
#include <string>
#include "Graphics.h"

struct StartUpData
{
	int WindowWidth;
	int WindowHeight;
	std::string GridFileName;
};

class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& InFileName);
	static StartUpData LoadLeveL(const std::string& InFileName);
};