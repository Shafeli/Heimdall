
#include <iostream>

#include <imgui.h>
#include "Core/Core.h"


class EditorCoreSystem final : public Brokkr::CoreSystems
{

public:

    virtual void Destroy() override;
};


void EditorCoreSystem::Destroy()
{
}

int main()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui::DestroyContext();

    Core::InitializeLua();
    Core::PrintHelloWorld();

	std::cout << "From Val Editor!\n";


	system("pause");
	return 0;

}
