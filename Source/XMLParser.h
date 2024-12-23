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
    std::string LevelFileName;
};

struct WorkshopData;
struct DecorationData;

class XMLParser
{
public:
    static GridConfig ParseGridDataFromXML(const std::string& InFileName);
    static StartUpData LoadLevel(const std::string& InFileName);
    static std::vector<WorkshopData> LoadWorkshops(const std::string& InFileName, const std::string& InCategoryName);
    static WorkshopData LoadWorkshop(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName);
    static std::vector<DecorationData> LoadDecorations(const std::string& InFileName, const std::string& InCategoryName);
    static DecorationData LoadDecoration(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName);
    static void UpdateGridValue(const std::string& InFileName, int InGridX, int InGridY, const char* InNewValue);
    static void ResetSave(const std::string& InFileName);
    static std::string GetGridValue(const std::string& InFileName, int InGridX, int InGridY);
    static void CheckInitEmptySave(const std::string& InFileName, int InGridX, int InGridY);
};


