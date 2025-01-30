project "ValkyrieEditor"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"

files
{
    "Source/**.h",
    "Source/**.cpp",
    "../lib/imgui/*.h",
    "../lib/imgui/*.cpp",
    "../lib/imgui/imstb_*.h",
    "../lib/imgui/backends/imgui_impl_sdl2.h",
    "../lib/imgui/backends/imgui_impl_sdl2.cpp",
    "../lib/imgui/backends/imgui_impl_vulkan.h",
    "../lib/imgui/backends/imgui_impl_vulkan.cpp"
}

includedirs
{
    "Source",
    "../BrokkrEngine/Source", -- Include BrokkrEngine
    "../BrokkrEngine/Source/Primitives",
    "../BrokkrEngine/Source/Utility",
    "../lib/imgui",
    "../lib/imgui/backends",
    "../lib/SDL2/include",   -- Include SDL2 headers
    "../lib/Vulkan/Include", -- Local Vulkan headers
}

libdirs
{
    "../lib/SDL2/lib/x64", -- SDL2 libraries
    "../lib/Vulkan/lib",   -- Local Vulkan libraries
    "vulkan-1",            -- Vulkan runtime library
}

links
{
    "BrokkrEngine",
    "SDL2",     -- Link SDL2
    "SDL2main", -- Link SDL2main
    "vulkan-1", -- Vulkan runtime library
}

-- Post-build commands to copy DLLs to the target directory
filter "configurations:Debug or Release or Dist"
postbuildcommands
{
    "xcopy \"$(SolutionDir)lib\\SDL2\\lib\\x64\\SDL2.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_image\\lib\\x64\\SDL2_image.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_image\\lib\\x64\\libpng16-16.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_image\\lib\\x64\\zlib1.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_image\\lib\\x64\\libjpeg-9.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_ttf\\lib\\x64\\SDL2_ttf.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_ttf\\lib\\x64\\libfreetype-6.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_mixer\\lib\\x64\\libmpg123-0.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\SDL_mixer\\lib\\x64\\SDL2_mixer.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\Vulkan\\Bin\\vulkan-1.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\Vulkan\\Bin\\VkLayer_*.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\Lua\\bin\\lua54.dll\" \"$(OutDir)\" /s /e /i /y",
    "xcopy \"$(SolutionDir)lib\\zlib\\contrib\\vstudio\\vc14\\x64\\ZlibStat$(Configuration)\\zlibstat.lib\" \"$(OutDir)\" /s /e /i /y"
}

targetdir("../Binaries/" .. OutputDir .. "/%{prj.name}")
objdir("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

filter "system:windows"
systemversion "latest"
defines { "WINDOWS" }

filter "configurations:Debug"
defines { "DEBUG" }
runtime "Debug"
symbols "On"

filter "configurations:Release"
defines { "RELEASE" }
runtime "Release"
optimize "On"
symbols "On"

filter "configurations:Dist"
defines { "DIST" }
runtime "Release"
optimize "On"
symbols "Off"
