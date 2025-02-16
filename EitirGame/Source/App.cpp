
#include <iostream>
#include <Core/Core.h>
#include <SceneManager/SceneManager.h>
#include <UnitTests/UnitTestSystem.h>
#include "Circle.h"
#include "2DPhysicsManager/PhysicsManager.h"
#include "2DRendering/SDLWindowSystem.h"
#include "AssetManager/AssetManager.h"
#include "Entity/GameEntityManager/GameEntityManager.h"
#include "UnitTests/UnitTest.h"
#include "XMLManager/XMLManager.h"

class GameCoreSystem final : public Brokkr::CoreSystems
{
	Brokkr::SDLWindowSystem* m_pSdlWindowManager = nullptr;
	Brokkr::AssetManager* m_pAssetManager = nullptr;
	Brokkr::SceneManager* m_pSceneManager = nullptr;
	Brokkr::UnitTestSystem* m_pUnitTestSystem = nullptr;
	Brokkr::GameEntityManager* m_pEntityManager = nullptr;
	Brokkr::PhysicsManager* m_pPhysicsManager2D = nullptr;
	Brokkr::XMLManager* m_pXmlManager = nullptr;

public:

	void Build()
	{
		m_pXmlManager = AddCoreSystem<Brokkr::XMLManager>();
		m_pSceneManager = AddCoreSystem<Brokkr::SceneManager>();
		m_pPhysicsManager2D = AddCoreSystem<Brokkr::PhysicsManager>();
		m_pSdlWindowManager = AddCoreSystem<Brokkr::SDLWindowSystem>();
		m_pEntityManager = AddCoreSystem<Brokkr::GameEntityManager>();

		auto pMainWindow = m_pSdlWindowManager->AddWindow<Brokkr::SDLWindow>("GameWindow", 1024, 768);
		auto pMainRenderer = m_pSdlWindowManager->GetRendererForWindow(pMainWindow);

		m_pAssetManager = AddCoreSystem<Brokkr::AssetManager>(pMainRenderer, pMainWindow, Brokkr::EngineDefinitions::ASSETS_PATH);

		m_pUnitTestSystem = AddCoreSystem<Brokkr::UnitTestSystem>();

	}

	virtual void Initialize() override
	{
		CoreSystems::Initialize();

		m_pXmlManager->Init();
		// m_pSdlWindowManager->Init();
		m_pAssetManager->Init();
		// m_pSceneManager->Init();
		m_pPhysicsManager2D->Init();
		m_pEntityManager->Init();
		Brokkr::UnitTest::RegisterEngineVector2Tests(m_pUnitTestSystem);

	}

	void RunTests()
	{

		m_pUnitTestSystem->RunTests();
	}

	void Run()
	{
		while (isRunning)
		{
			Update();

			// Update logic
			// m_pInputManager->Update(&windowEvent); // update input system

			m_pSceneManager->UpdateActiveState();
			m_pEntityManager->UpdateEntities();
			m_pPhysicsManager2D->ProcessUpdate();
			m_pEntityManager->LateUpdateEntities();
			m_pEntityManager->RenderEntities();
		}

	}

	virtual void Destroy() override;
};


void GameCoreSystem::Destroy()
{
	CoreSystems::Destroy();
	m_pSdlWindowManager = nullptr;
}

int main()
{

	std::cout << "Welcome to Val Editor!\n";
	auto game = GameCoreSystem();

	game.Build();
	game.Initialize();
	game.RunTests();
	game.Run();
	game.Destroy();

	return 0;

}
