
#include <iostream>
#include "Circle.h"
#include "2DRendering/SDLWindowSystem.h"
#include "AssetManager/AssetManager.h"
#include <SceneManager/SceneManager.h>
#include "Core/Core.h"

class EditorCoreSystem final : public Brokkr::CoreSystems
{
	Brokkr::SDLWindowSystem* m_pSdlWindowManager = nullptr;
	Brokkr::AssetManager* m_pAssetManager = nullptr;
	Brokkr::SceneManager* m_pSceneManager = nullptr;

public:
	virtual void Initialize() override
	{
		CoreSystems::Initialize();

		m_pSdlWindowManager->AddWindow<Brokkr::SDLWindow>("EditorWindowMain", 1024, 768);
	}

	void Build()
	{
		m_pSceneManager = AddCoreSystem<Brokkr::SceneManager>();
		m_pSdlWindowManager = AddCoreSystem<Brokkr::SDLWindowSystem>();

	}

	void Run()
	{
        while (isRunning)
        {
			Update();

			// Update logic here:

        }
		
	}

    virtual void Destroy() override;
};


void EditorCoreSystem::Destroy()
{
	CoreSystems::Destroy();
	m_pSdlWindowManager = nullptr;
}

int main()
{

	std::cout << "Welcome to Val Editor!\n";
	auto Editor = EditorCoreSystem();

	Editor.Build();

	Editor.Initialize();
	Editor.Run();
	Editor.Destroy();

	return 0;

}
