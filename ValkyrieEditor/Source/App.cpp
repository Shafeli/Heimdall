
#include <iostream>
#include "Core/Core.h"
#include "Primitives/Circle.h"
#include "SDLWindow/SDLWindowSystem.h"

class EditorCoreSystem final : public Brokkr::CoreSystems
{
	Brokkr::SDLWindowSystem* m_sdlWindowManager = nullptr;
public:
	virtual void Initialize() override
	{
		CoreSystems::Initialize();
	}

	void Build()
	{
		m_sdlWindowManager = AddCoreSystem<Brokkr::SDLWindowSystem>();
		m_sdlWindowManager->AddWindow<Brokkr::SDLWindow>("EditorWindowMain", 1024, 768);
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
	m_sdlWindowManager = nullptr;
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
