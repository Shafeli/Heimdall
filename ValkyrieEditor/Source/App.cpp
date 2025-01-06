
#include <iostream>

#include <imgui.h>
#include "Core/Core.h"

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