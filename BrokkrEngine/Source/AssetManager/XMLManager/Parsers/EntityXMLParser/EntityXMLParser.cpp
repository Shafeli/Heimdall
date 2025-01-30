#include "EntityXMLParser.h"

#include "ColliderComponent.h"
#include "tinyxml2.h"
#include "TransformComponent.h"
#include "Core/Core.h" 
#include "Entity/GameEntityManager/GameEntityManager.h"


Brokkr::EntityXMLParser::EntityXMLParser(CoreSystems* pCoreSystems): XMLParser(pCoreSystems)
{
    // Building functions
    ColliderComponent::RegisterCreationFunction(this);
    TransformComponent::RegisterCreationFunction(this);


}

// TODO: Build a abstract factory so components can register building methods 
bool Brokkr::EntityXMLParser::Parse(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Entity" element
    if (!root || strcmp(root->Name(), "Entity") != 0)
    {
        // The XML file is not a valid Entity file
        return false;
    }

    return true;
}

Brokkr::GameEntity* Brokkr::EntityXMLParser::BuildEntity(const std::string& entityName, const char* fileName)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName) != tinyxml2::XML_SUCCESS)
    {
        // Error handling
        return nullptr;
    }

    tinyxml2::XMLElement* root = doc.RootElement();

    // Check if the root element is a "Entity" element
    if (!root || std::string(root->Name()) != "Entity")
    {
        // The XML file is not a valid Entity file
        return nullptr;
    }

    // Loop through the entities
    for (tinyxml2::XMLElement* entityElement = root->FirstChildElement("GameEntity");
        entityElement != nullptr;
        entityElement = entityElement->NextSiblingElement("GameEntity"))
    {
        const char* nameAttr = entityElement->Attribute("name");
        if (std::string(nameAttr) == entityName)
        {
            // Create a new entity
            GameEntity* pEntity = m_pCoreSystems->GetCoreSystem<GameEntityManager>()->GetNextEntityAvailable();

            // Loop through the components of the element
            for (tinyxml2::XMLElement* pComponentElement = entityElement->FirstChildElement();
                pComponentElement != nullptr;
                pComponentElement = pComponentElement->NextSiblingElement())
            {
                const std::string componentName = pComponentElement->Name();
                auto iterator = m_componentConstructors.find(componentName);
                if (iterator != m_componentConstructors.end())
                {
                    iterator->second(pEntity, pComponentElement, m_pCoreSystems);
                }
            }
            return pEntity;
        }
    }

    return nullptr; // failed
}