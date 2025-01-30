#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Core/Core.h"

#include "Rectangle.h"
#include "Vector2.h"


// TODO: GameEntityManager


namespace Brokkr
{
    class PhysicsManager;
    class GameEntity;
    class EntityXMLParser;
    class PositionDataParser;
    class XMLManager;

    class GameEntityManager final : public System
    {
        std::vector<GameEntity*> m_entities;
        std::unordered_map<int, size_t> m_entityLookup;

        // Prefab system needs to know the parsers to get the data
        EntityXMLParser* m_pEntityParser = nullptr;
        PositionDataParser* m_pPositionParser = nullptr;
        XMLManager* m_pXmlManager = nullptr;
        PhysicsManager* m_pPhysicsManager = nullptr;

        std::unordered_map<std::string, Vector2<float>> m_entitiesStartingPositions;

    public:

        explicit GameEntityManager(CoreSystems* pCoreManager);

        void Init();

        // Objects Reset
        ///////////////////////////////////////////
        void ResetEntities() const;

        // Create Object Creation / Deletion 
        ///////////////////////////////////////////
        GameEntity* GetNextEntityAvailable();

        void DeleteEntity(int entityID);

        // Object Update Components
        ///////////////////////////////////////////
        void UpdateEntities() const;

        // Object Rendering
        ///////////////////////////////////////////
        void RenderEntities() const;

        GameEntity* GetEntityById(int entityID);

        std::vector<GameEntity*> GetEntitiesInArea(const Rectangle<float>& area);

        // Given a name of a prefab build all in the location data
        ///////////////////////////////////////////
        std::list<GameEntity*> ConstructWithLocation(const char* prefabName, const char* prefabFileName, const char* mapFileName);
        GameEntity* Construct(const char* prefabName, const char* fileName);

        void ClearEntities();
        virtual void Destroy() override;
        virtual ~GameEntityManager() override;
    };
}
