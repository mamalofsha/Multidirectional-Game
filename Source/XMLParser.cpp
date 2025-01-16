#include "XMLParser.h"
#include <vector>
#include "HUD.h"

GridConfig XMLParser::ParseGridDataFromXML(const std::string& InFileName)
{
    GridConfig OutConfig{};
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutConfig;
    }
    auto* Root = Doc.FirstChildElement("grid");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <grid> element." << std::endl;
        return OutConfig;
    }
    auto* Dimensions = Root->FirstChildElement("dimensions");
    if (Dimensions) {
        Dimensions->FirstChildElement("width")->QueryIntText(&OutConfig.Width);
        Dimensions->FirstChildElement("height")->QueryIntText(&OutConfig.Height);
    }
    auto* Tile = Root->FirstChildElement("tile");
    if (Tile) {
        Tile->FirstChildElement("width")->QueryFloatText(&OutConfig.TileWidth);
        Tile->FirstChildElement("height")->QueryFloatText(&OutConfig.TileHeight);
    }
    auto* Offset = Root->FirstChildElement("offset");
    if (Offset) {
        Offset->FirstChildElement("x")->QueryFloatText(&OutConfig.StartOffsetX);
        Offset->FirstChildElement("y")->QueryFloatText(&OutConfig.StartOffsetY);
    }
    return OutConfig;
}

StartUpData XMLParser::LoadLevel(const std::string& InFileName)
{
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return StartUpData();
    }
    auto* Root = Doc.FirstChildElement("level");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <level> element." << std::endl;
        return StartUpData();
    }
    StartUpData Data;
    auto* Asset = Root->FirstChildElement("asset");
    if (Asset) {
        Data.LevelFileName = Asset->GetText();
    }
    auto* Dimensions = Root->FirstChildElement("dimensions");
    if (Dimensions) {
        Dimensions->FirstChildElement("width")->QueryIntText(&Data.LevelWidth);
        Dimensions->FirstChildElement("height")->QueryIntText(&Data.LevelHeight);
    }
    auto* GridFile = Root->FirstChildElement("gridfilename");
    if (GridFile) {
        Data.GridFileName = GridFile->GetText();
    }
    auto* WindowScale = Root->FirstChildElement("WindowScale");
    if (WindowScale) {
        WindowScale->QueryFloatText(&Data.WindowScale);
    }
    return Data;
}

std::vector<WorkshopData> XMLParser::LoadWorkshops(const std::string& InFileName, const std::string& InCategoryName)
{
    std::vector<WorkshopData> OutData;
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* Root = Doc.FirstChildElement("items");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <items> element." << std::endl;
        return OutData;
    }
    auto* Category = Root->FirstChildElement(InCategoryName.c_str());
    if (!Category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }
    for (auto* Item = Category->FirstChildElement(); Item; Item = Item->NextSiblingElement()) {
        WorkshopData Data;
        Data.ItemID = Item->Name();
        auto* NameElement = Item->FirstChildElement("name");
        if (NameElement && NameElement->GetText()) {
            Data.Name = NameElement->GetText();
        }
        auto* ImageElement = Item->FirstChildElement("image");
        if (ImageElement && ImageElement->GetText()) {
            Data.ImageFile = ImageElement->GetText();
        }
        auto* GoldCostElement = Item->FirstChildElement("goldcost");
        if (GoldCostElement) {
            GoldCostElement->QueryFloatText(&Data.GoldCost);
        }
        auto* GoldGenElement = Item->FirstChildElement("goldgenrate");
        if (GoldGenElement) {
            GoldGenElement->QueryFloatText(&Data.GoldGenRate);
        }
        OutData.push_back(Data);
    }
    return OutData;
}

WorkshopData XMLParser::LoadWorkshop(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName)
{
    WorkshopData OutData;
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* Root = Doc.FirstChildElement("items");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <items> element." << std::endl;
        return OutData;
    }
    auto* Category = Root->FirstChildElement(InCategoryName.c_str());
    if (!Category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }
    for (auto* Item = Category->FirstChildElement(); Item; Item = Item->NextSiblingElement()) {
        if (Item->Name() != InItemName) continue;
        OutData.ItemID = Item->Name();
        auto* NameElement = Item->FirstChildElement("name");
        if (NameElement && NameElement->GetText()) {
            OutData.Name = NameElement->GetText();
        }
        auto* ImageElement = Item->FirstChildElement("image");
        if (ImageElement && ImageElement->GetText()) {
            OutData.ImageFile = ImageElement->GetText();
        }
        auto* GoldCostElement = Item->FirstChildElement("goldcost");
        if (GoldCostElement) {
            GoldCostElement->QueryFloatText(&OutData.GoldCost);
        }
        auto* GoldGenElement = Item->FirstChildElement("goldgenrate");
        if (GoldGenElement) {
            GoldGenElement->QueryFloatText(&OutData.GoldGenRate);
        }
        break;
    }
    return OutData;
}

DecorationData XMLParser::LoadDecoration(const std::string& InFileName, const std::string& InCategoryName, const std::string& InItemName)
{
    DecorationData OutData;
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* Root = Doc.FirstChildElement("items");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <items> element." << std::endl;
        return OutData;
    }
    auto* Category = Root->FirstChildElement(InCategoryName.c_str());
    if (!Category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }
    for (auto* Item = Category->FirstChildElement(); Item; Item = Item->NextSiblingElement()) {
        if (Item->Name() != InItemName) continue;
        OutData.ItemID = Item->Name();
        auto* NameElement = Item->FirstChildElement("name");
        if (NameElement && NameElement->GetText()) {
            OutData.Name = NameElement->GetText();
        }
        auto* ImageElement = Item->FirstChildElement("image");
        if (ImageElement && ImageElement->GetText()) {
            OutData.ImageFile = ImageElement->GetText();
        }
        auto* GoldCostElement = Item->FirstChildElement("goldcost");
        if (GoldCostElement) {
            GoldCostElement->QueryFloatText(&OutData.GoldCost);
        }
        auto* HappinessElement = Item->FirstChildElement("happiness");
        if (HappinessElement) {
            HappinessElement->QueryFloatText(&OutData.Happiness);
        }
        break;
    }
    return OutData;
}

std::vector<DecorationData> XMLParser::LoadDecorations(const std::string& InFileName, const std::string& InCategoryName)
{
    std::vector<DecorationData> OutData;
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* Root = Doc.FirstChildElement("items");
    if (!Root) {
        std::cerr << "Invalid XML structure: No <items> element." << std::endl;
        return OutData;
    }
    auto* Category = Root->FirstChildElement(InCategoryName.c_str());
    if (!Category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }
    for (auto* Item = Category->FirstChildElement(); Item; Item = Item->NextSiblingElement()) {
        DecorationData Data;
        Data.ItemID = Item->Name();
        auto* NameElement = Item->FirstChildElement("name");
        if (NameElement && NameElement->GetText()) {
            Data.Name = NameElement->GetText();
        }
        auto* ImageElement = Item->FirstChildElement("image");
        if (ImageElement && ImageElement->GetText()) {
            Data.ImageFile = ImageElement->GetText();
        }
        auto* GoldCostElement = Item->FirstChildElement("goldcost");
        if (GoldCostElement) {
            GoldCostElement->QueryFloatText(&Data.GoldCost);
        }
        auto* HappinessElement = Item->FirstChildElement("happiness");
        if (HappinessElement) {
            HappinessElement->QueryFloatText(&Data.Happiness);
        }
        OutData.push_back(Data);
    }
    return OutData;
}

void XMLParser::UpdateGridValue(const std::string& InFileName, int InGridX, int InGridY, const char* InNewValue)
{
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << InFileName << std::endl;
        return;
    }
    auto* Root = Doc.FirstChildElement("grid");
    if (!Root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return;
    }
    auto* Tiles = Root->FirstChildElement("tiles");
    if (!Tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return;
    }
    auto* RowElement = Tiles->FirstChildElement("row");
    for (int i = 0; i < InGridY; ++i) {
        if (RowElement) {
            RowElement = RowElement->NextSiblingElement("row");
        }
        else {
            std::cerr << "Error: Row index out of range." << std::endl;
            return;
        }
    }
    if (!RowElement) {
        std::cerr << "Error: Row not found." << std::endl;
        return;
    }
    const char* RowText = RowElement->GetText();
    if (!RowText) {
        std::cerr << "Error: Row is empty." << std::endl;
        return;
    }
    std::stringstream SS(RowText);
    std::vector<std::string> RowValues;
    std::string Value;
    while (SS >> Value) {
        RowValues.push_back(Value);
    }
    if (InGridX < 0 || InGridX >= static_cast<int>(RowValues.size())) {
        std::cerr << "Error: Column index out of range." << std::endl;
        return;
    }
    RowValues[InGridX] = InNewValue;
    std::ostringstream NewRowText;
    for (size_t i = 0; i < RowValues.size(); ++i) {
        NewRowText << RowValues[i];
        if (i < RowValues.size() - 1) {
            NewRowText << " ";
        }
    }
    RowElement->SetText(NewRowText.str().c_str());
    if (Doc.SaveFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to save XML file." << std::endl;
    }
    else {
        std::cout << "Grid updated successfully." << std::endl;
    }
}

void XMLParser::ResetSave(const std::string& InFileName)
{
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << InFileName << std::endl;
        return;
    }
    auto* Root = Doc.FirstChildElement("grid");
    if (!Root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return;
    }
    auto* Tiles = Root->FirstChildElement("tiles");
    if (!Tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return;
    }
    auto* RowElement = Tiles->FirstChildElement("row");
    while (RowElement) {
        const char* RowText = RowElement->GetText();
        if (RowText) {
            std::stringstream SS(RowText);
            std::ostringstream NewRow;
            std::string Value;
            bool Modified = false;
            while (SS >> Value) {
                if (Value != "0") {
                    Value = "0";
                    Modified = true;
                }
                NewRow << Value << " ";
            }
            std::string UpdatedRow = NewRow.str();
            if (!UpdatedRow.empty() && UpdatedRow.back() == ' ') {
                UpdatedRow.pop_back();
            }
            if (Modified) {
                RowElement->SetText(UpdatedRow.c_str());
            }
        }
        RowElement = RowElement->NextSiblingElement("row");
    }
    if (Doc.SaveFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to save XML file." << std::endl;
    }
    else {
        std::cout << "Non-zero values have been replaced with 0 successfully." << std::endl;
    }
}

std::string XMLParser::GetGridValue(const std::string& InFileName, int InGridX, int InGridY)
{
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << InFileName << std::endl;
        return std::string();
    }
    auto* Root = Doc.FirstChildElement("grid");
    if (!Root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return std::string();
    }
    auto* Tiles = Root->FirstChildElement("tiles");
    if (!Tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return std::string();
    }
    auto* RowElement = Tiles->FirstChildElement("row");
    for (int i = 0; i < InGridY; ++i) {
        if (RowElement) {
            RowElement = RowElement->NextSiblingElement("row");
        }
        else {
            std::cerr << "Error: Row index out of range." << std::endl;
            return std::string();
        }
    }
    if (!RowElement) {
        std::cerr << "Error: Row not found." << std::endl;
        return std::string();
    }
    const char* RowText = RowElement->GetText();
    if (!RowText) {
        std::cerr << "Error: Row is empty." << std::endl;
        return std::string();
    }
    std::stringstream SS(RowText);
    std::vector<std::string> RowValues;
    std::string Value;
    while (SS >> Value) {
        RowValues.push_back(Value);
    }
    if (InGridX < 0 || InGridX >= static_cast<int>(RowValues.size())) {
        std::cerr << "Error: Column index out of range." << std::endl;
        return std::string();
    }
    return RowValues[InGridX];
}

void XMLParser::CheckInitEmptySave(const std::string& InFileName, int InGridX, int InGridY)
{
    XMLDocument Doc;
    if (Doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << InFileName << std::endl;
        return;
    }
    auto* Root = Doc.FirstChildElement("grid");
    if (!Root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return;
    }
    auto* Tiles = Root->FirstChildElement("tiles");
    if (!Tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return;
    }
    auto* RowElement = Tiles->FirstChildElement("row");
    if (RowElement) {
        std::cout << "Save file already exists with rows." << std::endl;
        return;
    }
    else {
        for (int i = 0; i < InGridY; ++i) {
            auto* NewRow = Doc.NewElement("row");
            std::ostringstream RowContent;
            for (int j = 0; j < InGridX; ++j) {
                RowContent << "0";
                if (j < InGridX - 1)
                    RowContent << " ";
            }
            std::string RowString = RowContent.str();
            NewRow->SetText(RowString.c_str());
            Tiles->InsertEndChild(NewRow);
        }
        if (Doc.SaveFile(InFileName.c_str()) != XML_SUCCESS) {
            std::cerr << "Error: Unable to save initialized grid to XML file." << std::endl;
        }
        else {
            std::cout << "Grid initialized successfully." << std::endl;
        }
    }
}


