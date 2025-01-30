#include "GameEntityManager.h"

#include <ColliderComponent.h>
#include <GameEntity.h>
#include <TransformComponent.h>
#include "AssetManager/AssetManager.h"
#include "AssetManager/XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"
#include "AssetManager/XMLManager/Parsers/PositionDataParser/PositionDataParser.h"


namespace Brokkr
{
    class ColliderComponent;
    class AssetManager;
}

Brokkr::GameEntityManager::GameEntityManager(CoreSystems* pCoreManager): System(pCoreManager)
{
    //
}

Brokkr::GameEntityManager::~GameEntityManager()
{
    for (const auto& pEntity: m_entities)
    {
        delete pEntity;
    }
}

void Brokkr::GameEntityManager::Init()
{
    m_pPhysicsManager = m_pCoreManager->GetCoreSystem<PhysicsManager>();

}

void Brokkr::GameEntityManager::ResetEntities() const
{
    for (const auto& pEntity: m_entities)
    {
        pEntity->Enable();
    }
}

Brokkr::GameEntity* Brokkr::GameEntityManager::GetNextEntityAvailable()
{
    auto pEntity = new GameEntity;

    m_entities.push_back(pEntity);
    m_entityLookup[pEntity->GetId()] = m_entities.size() - 1;

    return pEntity;
}

void Brokkr::GameEntityManager::DeleteEntity(int entityID)
{
    const auto it = m_entityLookup.find(entityID);
    if (it == m_entityLookup.end())
    {
        return;
    }

    const size_t index = it->second;

    // Delete the entity being removed
    delete m_entities[index];

    // Swap the entity to be deleted with the last entity in the vector
    m_entities[index] = std::move(m_entities.back());

    // Update the lookup map
    m_entityLookup[m_entities[index]->GetId()] = index;

    // Remove the last entity in the vector
    m_entities.pop_back();

    // Remove the entity from the lookup map
    m_entityLookup.erase(it);
}

void Brokkr::GameEntityManager::UpdateEntities() const
{
    for (auto& pEntity : m_entities)
    {
        pEntity->Update();
    }
}

void Brokkr::GameEntityManager::RenderEntities() const
{
    for (auto& pEntity : m_entities)
    {
        pEntity->Render();
    }
}

Brokkr::GameEntity* Brokkr::GameEntityManager::GetEntityById(int entityID)
{
    auto it = m_entityLookup.find(entityID);
    if (it == m_entityLookup.end())
    {
        return nullptr;
    }

    return m_entities[it->second];
}

std::vector<Brokkr::GameEntity*> Brokkr::GameEntityManager::GetEntitiesInArea(const Rectangle<float>& area)
{
    const auto objectIds = m_pPhysicsManager->QueryAreaAll(area);

    if (objectIds.empty())
        return {}; // No Objects Found

    std::vector<GameEntity*> pEntities;
    pEntities.reserve(objectIds.size());

    for (const int objectId : objectIds)
    {
        if (GameEntity* pEntity = GetEntityById(objectId))
        {
            pEntities.push_back(pEntity);
        }
    }

    return pEntities;
}

std::list<Brokkr::GameEntity*> Brokkr::GameEntityManager::ConstructWithLocation(const char* prefabName, const char* prefabFileName, const char* mapFileName)
{
    if (!m_pXmlManager) // get xml Manager
    {
        m_pXmlManager = m_pCoreManager->GetCoreSystem<AssetManager>()->GetAssetSystem<XMLManager>();
    }

    if (!m_pEntityParser) // get the parser 
    {
        m_pEntityParser = m_pXmlManager->GetParser<EntityXMLParser>();
    }
    if (!m_pPositionParser)
    {
        m_pPositionParser = m_pXmlManager->GetParser<PositionDataParser>();
    }

    // get data from xml manager
    const char* mapData = m_pXmlManager->Get(mapFileName);

    // parse the data
    const auto posData = m_pPositionParser->ParseLayer(prefabName, mapData);

    // Create an init list
    std::list<GameEntity*> pInitList;
    // create entities based on how many are in the location data
    for (const auto data : posData)
    {
        pInitList.emplace_back(Construct(prefabName, prefabFileName));

        // add location data to the entities
        const auto pTransform = pInitList.back()->GetComponent<TransformComponent>();
        pTransform->MoveTo(data);
        pTransform->SetStartingPos(data);

        const auto pCollider = pInitList.back()->GetComponent<ColliderComponent>();

        if (pCollider != nullptr)
            pCollider->AdjustByPos(data);

    }

    for (const auto& i : pInitList)
    {
        i->Init();
    }

    // and if all succeeded then return true
    return pInitList;
}

Brokkr::GameEntity* Brokkr::GameEntityManager::Construct(const char* prefabName, const char* fileName)
{
    if (!m_pXmlManager) // get xml Manager
    {
        m_pXmlManager = m_pCoreManager->GetCoreSystem<AssetManager>()->GetAssetSystem<XMLManager>();
    }

    if (!m_pEntityParser) // get the parser 
    {
        m_pEntityParser = m_pXmlManager->GetParser<EntityXMLParser>();
    }

    const auto init = m_pEntityParser->BuildEntity(prefabName, m_pXmlManager->Get(fileName));
    init->Init();

    return init;
}

void Brokkr::GameEntityManager::ClearEntities()
{
    try
    {
        for (auto& entity : m_entities)
        {
            if (entity != nullptr)
            {
                delete entity;
                entity = nullptr;
            }
        }
        m_entities.clear();
        m_entityLookup.clear();
    }
    catch ([[maybe_unused]] const std::exception& e)
    {
        // Log the exception message
        // m_fileLog.Log(Logger::LogLevel::kError, std::string("Exception in ClearEntities: ") + e.what());
    }
}

void Brokkr::GameEntityManager::Destroy()
{
    //TODO:
    ClearEntities();
}

