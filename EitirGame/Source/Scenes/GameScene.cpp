#include "GameScene.h"

#include "2DPhysicsManager/PhysicsManager.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/2DTextureManager/TextureManager2D.h"
#include "Core/EngineDefinitions.h"
#include "Entity/GameEntityManager/GameEntityManager.h"
#include "XMLManager/XMLManager.h"
void GameScene::Init()
{
    m_pXmlManager = m_pCoreSystems->GetCoreSystem<Brokkr::XMLManager>();
    m_pEntityManager = m_pCoreSystems->GetCoreSystem<Brokkr::GameEntityManager>();
    m_pPhysicsManager = m_pCoreSystems->GetCoreSystem<Brokkr::PhysicsManager>();

    std::string hackyXMLPath = Brokkr::EngineDefinitions::ASSETS_PATH;
    hackyXMLPath += "GameScene/GameEntities.XML";
    m_pXmlManager->Load(hackyXMLPath.c_str());

    hackyXMLPath.clear();
    hackyXMLPath = Brokkr::EngineDefinitions::ASSETS_PATH;
    hackyXMLPath += "GameScene/GameScene.tmx";
    m_pXmlManager->Load(hackyXMLPath.c_str());

    const auto pAssetSystem = m_pCoreSystems->GetCoreSystem<Brokkr::AssetManager>();
    const auto pTextureManager = pAssetSystem->GetAssetSystem<Brokkr::TextureManager2D>();

    hackyXMLPath.clear();
    hackyXMLPath = Brokkr::EngineDefinitions::ASSETS_PATH;
    hackyXMLPath += "GameScene/Player.png";
    pTextureManager->LoadTexture("Player", hackyXMLPath.c_str());

    hackyXMLPath.clear();
    hackyXMLPath = Brokkr::EngineDefinitions::ASSETS_PATH;
    hackyXMLPath += "GameScene/AI.png";
    pTextureManager->LoadTexture("AI", hackyXMLPath.c_str());

    hackyXMLPath.clear();
    hackyXMLPath = Brokkr::EngineDefinitions::ASSETS_PATH;
    hackyXMLPath += "GameScene/Square.png";
    pTextureManager->LoadTexture("Square", hackyXMLPath.c_str());
}

void GameScene::Update()
{

}

void GameScene::Exit()
{
}

void GameScene::Enter()
{
    
    m_pPhysicsManager->SetWorldSize({ 1024.f, 768.f });
    m_pEntityManager->ConstructWithLocation("Collider", "GameEntities.XML", "GameScene.tmx");
    m_pEntityManager->ConstructWithLocation("DoorWay", "GameEntities.XML", "GameScene.tmx");
    m_pEntityManager->ConstructWithLocation("FinishLine", "GameEntities.XML", "GameScene.tmx");

    m_pEntityManager->ConstructWithLocation("Player", "GameEntities.XML", "GameScene.tmx");
    m_pEntityManager->ConstructWithLocation("AI", "GameEntities.XML", "GameScene.tmx");

}

void GameScene::Destroy()
{
}
