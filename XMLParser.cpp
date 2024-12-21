#include "XMLParser.h"
#include <vector>
#include "Graphics.h"

GridConfig XMLParser::ParseGridDataFromXML(const std::string& InFileName)
{
    GridConfig OutConfig;
    // Load XML file
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(InFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutConfig;
    }

    // Parse dimensions
    auto* root = doc.FirstChildElement("grid");
    if (!root) {
        std::cerr << "Invalid XML structure: No <grid> element." << std::endl;
        return OutConfig;
    }

    auto* dimensions = root->FirstChildElement("dimensions");
    if (dimensions) {
        dimensions->FirstChildElement("width")->QueryIntText(&OutConfig.width);
        dimensions->FirstChildElement("height")->QueryIntText(&OutConfig.height);
    }

    // Parse tile size
    auto* tile = root->FirstChildElement("tile");
    if (tile) {
        tile->FirstChildElement("width")->QueryFloatText(&OutConfig.tileWidth);
        tile->FirstChildElement("height")->QueryFloatText(&OutConfig.tileHeight);
    }

    // Parse Offset
    auto* offset = root->FirstChildElement("offset");
    if (offset) {
        offset->FirstChildElement("x")->QueryFloatText(&OutConfig.StartOffsetX);
        offset->FirstChildElement("y")->QueryFloatText(&OutConfig.StartOffsetY);
    }

    // Parse tiles
    auto* tilesElement = root->FirstChildElement("tiles");
    if (tilesElement) {
        for (auto* row = tilesElement->FirstChildElement("row"); row; row = row->NextSiblingElement("row")) {
            std::vector<int> tileRow;
            const char* rowText = row->GetText();
            std::stringstream ss(rowText); // Initialize with rowText
            int tileType;
            while (ss >> tileType) {
                tileRow.push_back(tileType);
            }
            OutConfig.tiles.push_back(tileRow);
        }
    }

    return OutConfig;
}

StartUpData XMLParser::LoadLeveL(const std::string& InFileName)
{
    StartUpData Data;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(InFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return Data;
    }
    auto* root = doc.FirstChildElement("level");
    if (!root) {
        std::cerr << "Invalid XML structure: No <level> element." << std::endl;
        return Data;
    }

    auto* dimensions = root->FirstChildElement("dimensions");
    if (dimensions) {
        dimensions->FirstChildElement("width")->QueryIntText(&Data.LevelWidth);
        dimensions->FirstChildElement("height")->QueryIntText(&Data.LevelHeight);
    }

    auto* GridFile = root->FirstChildElement("gridfilename");
    if (GridFile) {
        Data.GridFileName= GridFile->GetText();
    }

    auto* WindowScale = root->FirstChildElement("WindowScale");
    if (WindowScale) {
        float scaleValue = 0.0f;
        if (WindowScale->QueryFloatText(&scaleValue) == tinyxml2::XML_SUCCESS) {
            Data.WindowScale = scaleValue; // Assign only on success
        }
        else {
            std::cerr << "Error: Failed to parse WindowScale as a float.\n";
        }
    }
    return Data;
}

std::vector<WorkshopData> XMLParser::LoadWorkShops(const std::string& InFileName, const std::string& InCategoryName)
{
    std::vector<WorkshopData> OutData;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(InFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* root = doc.FirstChildElement("items");
    if (!root) {
        std::cerr << "Invalid XML structure: No <level> element." << std::endl;
        return OutData;
    }
    // Locate the specified category (e.g., "workshops")
    auto* category = root->FirstChildElement(InCategoryName.c_str());
    if (!category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }

    // Iterate through the items in the category
    for (auto* item = category->FirstChildElement(); item; item = item->NextSiblingElement()) {
        WorkshopData data;

        // Parse name
        auto* nameElement = item->FirstChildElement("name");
        if (nameElement && nameElement->GetText()) {
            data.Name = nameElement->GetText();
        }

        // Parse image file
        auto* imageElement = item->FirstChildElement("image");
        if (imageElement && imageElement->GetText()) {
            data.ImageFile = imageElement->GetText();
        }

        // Parse gold cost
        auto* goldCostElement = item->FirstChildElement("goldcost");
        if (goldCostElement) {
            goldCostElement->QueryFloatText(&data.goldCost);
        }

        // Parse gold generation rate
        auto* goldGenElement = item->FirstChildElement("goldgenrate");
        if (goldGenElement) {
            goldGenElement->QueryFloatText(&data.goldGenrate);
        }

        // Add the parsed data to the output vector
        OutData.push_back(data);
    }

    return OutData;
}

std::vector<Decoration> XMLParser::LoadDecorations(const std::string& InFileName, const std::string& InCategoryName)
{
    std::vector<Decoration> OutData;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(InFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << InFileName << std::endl;
        return OutData;
    }
    auto* root = doc.FirstChildElement("items");
    if (!root) {
        std::cerr << "Invalid XML structure: No <level> element." << std::endl;
        return OutData;
    }
    // Locate the specified category (e.g., "workshops")
    auto* category = root->FirstChildElement(InCategoryName.c_str());
    if (!category) {
        std::cerr << "Category " << InCategoryName << " not found in XML." << std::endl;
        return OutData;
    }

    // Iterate through the items in the category
    for (auto* item = category->FirstChildElement(); item; item = item->NextSiblingElement()) {
        Decoration data;

        // Parse name
        auto* nameElement = item->FirstChildElement("name");
        if (nameElement && nameElement->GetText()) {
            data.Name = nameElement->GetText();
        }

        // Parse image file
        auto* imageElement = item->FirstChildElement("image");
        if (imageElement && imageElement->GetText()) {
            data.ImageFile = imageElement->GetText();
        }

        // Parse gold cost
        auto* goldCostElement = item->FirstChildElement("goldcost");
        if (goldCostElement) {
            goldCostElement->QueryFloatText(&data.goldCost);
        }

        // Parse gold generation rate
        auto* happyElement = item->FirstChildElement("happiness");
        if (happyElement) {
            happyElement->QueryFloatText(&data.happiness);
        }

        // Add the parsed data to the output vector
        OutData.push_back(data);
    }

    return OutData;
}


