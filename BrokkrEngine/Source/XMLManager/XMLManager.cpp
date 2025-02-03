#include "XMLManager.h"

#include <filesystem>

#include "tinyxml2.h"
#include "Parsers/EntityXMLParser/EntityXMLParser.h"
#include "Parsers/PositionDataParser/PositionDataParser.h"


void Brokkr::XMLManager::Init()
{
    AddParser<EntityXMLParser>(m_pCoreManager); // adding the Entity parser
    AddParser<PositionDataParser>(m_pCoreManager); // adding the Entity parser 
}

void Brokkr::XMLManager::Load(const char* filePath)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath) != tinyxml2::XML_SUCCESS)
    {
        // Error handling
        return;
    }

    namespace fs = std::filesystem;
    const fs::path path(filePath);
    const std::string fileName = path.filename().generic_string();

    if (!fileName.empty())
    {
        m_xmlPaths[fileName] = filePath;
    }
}

const char* Brokkr::XMLManager::Get(const std::string& fileName)
{
    if (const auto& xml = m_xmlPaths.find(fileName); xml != m_xmlPaths.end())
    {
        return xml->second;
    }

    return nullptr; // File not found
}

bool Brokkr::XMLManager::Parse(const char* fileName) const
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName) != tinyxml2::XML_SUCCESS)
    {
        // Error handling
        return false;
    }

    // Loop through the parsers until one of them successfull
    for (auto& parser : m_parsers)
    {
        if (parser->Parse(doc))
        {
            // Parsing successful
            return true;
        }
    }

    return false;
}

void Brokkr::XMLManager::Destroy()
{
    m_parsers.clear();
}

