
#include <iostream>
#include "Circle.h"
#include "2DRendering/SDLWindowSystem.h"
#include "AssetManager/AssetManager.h"
#include "Core/Core.h"

class EditorCoreSystem final : public Brokkr::CoreSystems
{
	Brokkr::SDLWindowSystem* m_pSdlWindowManager = nullptr;
	Brokkr::AssetManager* m_pAssetManager = nullptr;

public:
	virtual void Initialize() override
	{
		CoreSystems::Initialize();
	}

	void Build()
	{
		m_pSdlWindowManager = AddCoreSystem<Brokkr::SDLWindowSystem>();
		m_pSdlWindowManager->AddWindow<Brokkr::SDLWindow>("EditorWindowMain", 1024, 768);
	}

	void Run()
	{
        while (isRunning)
        {
			UpdateDelta();

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

	Editor.Initialize();
	Editor.Build();
	Editor.Run();
	Editor.Destroy();

	return 0;

}
