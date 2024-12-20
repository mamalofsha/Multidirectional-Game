#include "XMLParser.h"
#include <vector>
#include <TinyXML/tinyxml2.h>
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
        dimensions->FirstChildElement("width")->QueryIntText(&Data.WindowWidth);
        dimensions->FirstChildElement("height")->QueryIntText(&Data.WindowHeight);
    }

    auto* GridFile = root->FirstChildElement("gridfile");
    if (GridFile) {
        Data.GridFileName= GridFile->FirstChildElement("name")->GetText();
        std::cout << GridFile->FirstChildElement("name")->GetText();
    }
    return Data;
}
