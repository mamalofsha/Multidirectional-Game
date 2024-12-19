#pragma once
#include <string>
#include "Graphics.h"


class XMLParser
{
public:
	static GridConfig ParseGridDataFromXML(const std::string& filename);
};