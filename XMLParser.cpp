#include "XMLParser.h"
#include <vector>
#include "Graphics.h"
#include "HUD.h"

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
    XMLDocument doc;
    if (doc.LoadFile(InFileName.c_str()) != XML_SUCCESS) {
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

void XMLParser::UpdateGridValue(const std::string& filename, int gridX, int gridY, int newValue)
{
    tinyxml2::XMLDocument doc;

    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << filename << std::endl;
        return;
    }

    // Locate the <grid> element
    XMLElement* root = doc.FirstChildElement("grid");
    if (!root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return;
    }

    // Locate the <tiles> element
    XMLElement* tiles = root->FirstChildElement("tiles");
    if (!tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return;
    }

    // Navigate to the correct row
    XMLElement* rowElement = tiles->FirstChildElement("row");
    for (int i = 0; i < gridY; ++i) {
        if (rowElement) {
            rowElement = rowElement->NextSiblingElement("row");
        }
        else {
            std::cerr << "Error: Row index out of range." << std::endl;
            return;
        }
    }

    if (!rowElement) {
        std::cerr << "Error: Row not found." << std::endl;
        return;
    }

    // Parse the row into a vector
    const char* rowText = rowElement->GetText();
    if (!rowText) {
        std::cerr << "Error: Row is empty." << std::endl;
        return;
    }

    std::stringstream ss(rowText);
    std::vector<int> rowValues;
    int value;
    while (ss >> value) {
        rowValues.push_back(value);
    }

    // Check if the column index is valid
    if (gridX < 0 || gridX >= static_cast<int>(rowValues.size())) {
        std::cerr << "Error: Column index out of range." << std::endl;
        return;
    }

    // Update the value at the specified column
    rowValues[gridX] = newValue;

    // Convert the updated row back to a string
    std::ostringstream newRowText;
    for (size_t i = 0; i < rowValues.size(); ++i) {
        newRowText << rowValues[i];
        if (i < rowValues.size() - 1) {
            newRowText << " ";
        }
    }

    // Update the row text in the XML
    rowElement->SetText(newRowText.str().c_str());

    // Save the updated XML back to the file
    if (doc.SaveFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to save XML file." << std::endl;
    }
    else {
        std::cout << "Grid updated successfully." << std::endl;
    }
}

void XMLParser::ResetSave(const std::string& filename)
{
    {
        XMLDocument doc;

        // Load the XML file
        if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
            std::cerr << "Error: Unable to load XML file: " << filename << std::endl;
            return;
        }

        // Locate the <tiles> element
        XMLElement* root = doc.FirstChildElement("grid");
        if (!root) {
            std::cerr << "Error: No <grid> element found in XML." << std::endl;
            return;
        }

        XMLElement* tiles = root->FirstChildElement("tiles");
        if (!tiles) {
            std::cerr << "Error: No <tiles> element found in XML." << std::endl;
            return;
        }

        // Iterate over each <row> element
        XMLElement* rowElement = tiles->FirstChildElement("row");
        while (rowElement) {
            const char* rowText = rowElement->GetText();
            if (rowText) {
                // Replace 1s with 0s
                std::stringstream ss(rowText);
                std::ostringstream newRow;
                int value;
                while (ss >> value) {

                    value = 0;

                    newRow << value << " ";
                }

                // Remove the trailing space
                std::string updatedRow = newRow.str();
                if (!updatedRow.empty() && updatedRow.back() == ' ') {
                    updatedRow.pop_back();
                }

                // Update the row text in XML
                rowElement->SetText(updatedRow.c_str());
            }

            // Move to the next row
            rowElement = rowElement->NextSiblingElement("row");
        }

        // Save the updated XML back to the file
        if (doc.SaveFile(filename.c_str()) != XML_SUCCESS) {
            std::cerr << "Error: Unable to save XML file." << std::endl;
        }
        else {
            std::cout << "All 1s have been replaced with 0s successfully." << std::endl;
        }
    }
}

int XMLParser::GetGridValue(const std::string& filename, int gridX, int gridY)
{
   XMLDocument doc;

    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Unable to load XML file: " << filename << std::endl;
        return -1; // Return an invalid value to indicate an error
    }

    // Locate the <grid> element
    XMLElement* root = doc.FirstChildElement("grid");
    if (!root) {
        std::cerr << "Error: No <grid> element found in XML." << std::endl;
        return -1;
    }

    // Locate the <tiles> element
    XMLElement* tiles = root->FirstChildElement("tiles");
    if (!tiles) {
        std::cerr << "Error: No <tiles> element found in XML." << std::endl;
        return -1;
    }

    // Navigate to the correct row
    XMLElement* rowElement = tiles->FirstChildElement("row");
    for (int i = 0; i < gridY; ++i) {
        if (rowElement) {
            rowElement = rowElement->NextSiblingElement("row");
        }
        else {
            std::cerr << "Error: Row index out of range." << std::endl;
            return -1;
        }
    }

    if (!rowElement) {
        std::cerr << "Error: Row not found." << std::endl;
        return -1;
    }

    // Parse the row into a vector
    const char* rowText = rowElement->GetText();
    if (!rowText) {
        std::cerr << "Error: Row is empty." << std::endl;
        return -1;
    }

    std::stringstream ss(rowText);
    std::vector<int> rowValues;
    int value;
    while (ss >> value) {
        rowValues.push_back(value);
    }

    // Check if the column index is valid
    if (gridX < 0 || gridX >= static_cast<int>(rowValues.size())) {
        std::cerr << "Error: Column index out of range." << std::endl;
        return -1;
    }

    // Return the value at the specified column
    return rowValues[gridX];
}


