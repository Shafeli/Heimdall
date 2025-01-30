#include "PositionDataParser.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <tinyxml2.h>


Brokkr::PositionDataParser::~PositionDataParser()
{
    //
}

bool Brokkr::PositionDataParser::Parse(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* root = doc.RootElement();

    return false;
}

std::vector<Brokkr::Vector2<float>> Brokkr::PositionDataParser::ParseLayer(const char* layerName, const char* mapFile) const
{
    std::vector<Brokkr::Vector2<float>> layerData;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(mapFile) != tinyxml2::XML_SUCCESS)
    {
        // Error handling
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        std::string mapF = mapFile;
        return {};
    }

    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Map" element
    if (!root || strcmp(root->Name(), "map") != 0)
    {
        // The XML file is not a valid Map file
        return layerData;
    }

    // Properties
    auto tileWidth = root->IntAttribute("tilewidth");
    auto tileHeight = root->IntAttribute("tileheight");
    auto mapWidth = root->IntAttribute("width");
    auto mapHeight = root->IntAttribute("height");

    // Iterate through 
    for (tinyxml2::XMLElement* element = root->FirstChildElement(); element; element = element->NextSiblingElement())
    {
        // Check if the current element is a "layer" 
        if (strcmp(element->Name(), "layer") == 0 && strcmp(element->Attribute("name"), layerName) == 0)
        {
            // Process the current
            tinyxml2::XMLElement* dataElement = element->FirstChildElement("data");
            if (!dataElement)
            {
                // Handle error: CSV Data check failed for the specified layer

                return layerData;
            }

            // Parse the tile data as a CSV string
            const char* csvData = dataElement->GetText();
            std::istringstream stream(csvData);
            std::vector<int> tileValues;

            // Read each line of the CSV data
            std::string line;
            while (std::getline(stream, line))
            {
                // Remove the newline character from the end of the line
                if (!line.empty() && line[line.size() - 1] == '\n')
                {
                    line.erase(line.size() - 1);
                }

                // Parse the comma-separated values in the line
                std::istringstream lineStream(line);
                std::string value;
                while (std::getline(lineStream, value, ','))
                {
                    if (tileValues.size() == static_cast<size_t>(mapWidth * mapHeight))
                    {
                        break;
                    }
                    int intValue = std::stoi(value);
                    tileValues.push_back(intValue);
                }
            }

            // Calculations of the CSV array
            for (int y = 0; y < mapHeight; y++)
            {
                for (int x = 0; x < mapWidth; x++)
                {
                    // Retrieve the value of the tile at (x, y)
                    int tileValue = tileValues[y * mapWidth + x];

                    // Make a transform and set its position
                    if (tileValue != 0)
                    {
                        layerData.emplace_back(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
                    }
                }
            }

            break;
        }
    }

    return layerData;
}
