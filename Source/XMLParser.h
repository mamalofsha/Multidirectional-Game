#pragma once
#include <string>
#include "Graphics.h"
#include <TinyXML/tinyxml2.h>
using namespace tinyxml2;

// loading the level data from TestLevel.xml
struct StartUpData
{
    int LevelWidth;  // 4x4
    int LevelHeight;
    float WindowScale;            
    std::string GridFileName;   // grid config xml file
    std::string LevelFileName;  // bg asset path
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
     // checks to see if rows are empty so it makes an edit to the save file and init rows 
    //- for when generating the level with a new grid format ( it's width and length ) (2x2 example)
   //    <tiles>     -->     <tiles>
  //     </tiles>                  <row>0 0</row>
 //                                <row>0 0</row>
//                           </tiles>
    static void CheckInitEmptySave(const std::string& InFileName, int InGridX, int InGridY);
};


