
#include <iostream>
#include "Core/Core.h"
#include "SDLWindow/SDLWindowSystem.h"

class EditorCoreSystem final : public Brokkr::CoreSystems
{

public:
	void Init()
	{
		auto WindowManager = AddCoreSystem<SDLWindowSystem>();
		WindowManager->AddWindow<Brokkr::SDLWindow>("EditorWindowMain", 1024, 768);
	}
	void Run()
	{
        while (isRunning)
        {
			UpdateDelta();

			// Update logic
        }
		
	}

    virtual void Destroy() override;
};


void EditorCoreSystem::Destroy()
{
}

int main()
{

	std::cout << "From Val Editor!\n";
	auto Editor = EditorCoreSystem();

	Editor.Init();

	Editor.Run();
	system("pause");
	 
	return 0;

}
