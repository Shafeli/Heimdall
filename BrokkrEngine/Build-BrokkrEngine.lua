project "BrokkrEngine"
kind "StaticLib"
language "C++"
cppdialect "C++17"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"

files
{
    "Source/**.h",
    "Source/**.cpp",
    "../lib/TinyXML/**.h",
    "../lib/TinyXML/**.cpp",
    "../lib/GAP311/**.hpp",
}

includedirs
{
    "Source", -- source directory
    "Source/UnitTests",
    "Source/Core",
    "Source/Primitives",
    "Source/Entity/GameEntity",
    "Source/Entity/GameEntity/Component",
    "Source/Entity/GameEntity/Component/TransformComponent",
    "Source/Entity/GameEntity/Component/ColliderComponent",
    "Source/Entity/GameEntity/Component/RenderComponent/SpriteComponent",
    "../lib/SDL2/include",      -- SDL2 headers
    "../lib/SDL_image/include", -- SDL_image headers
    "../lib/SDL_mixer/include", -- SDL_mixer headers
    "../lib/SDL_ttf/include",   -- SDL_ttf headers
    "../lib/zlib",              -- zlib headers
    "../lib/Vulkan/Include",    -- Local Vulkan headers
    "../lib/TinyXML",           -- Local TinyXML headers
    "../lib/Lua/include",       -- Include Lua headers
    "../lib/GAP311",
    "../lib/vk-bootstrap/src"
}

libdirs
{
    "../lib/SDL2/lib/x64",      -- SDL2 libraries
    "../lib/SDL_image/lib/x64", -- SDL_image libraries
    "../lib/SDL_mixer/lib/x64", -- SDL_mixer libraries
    "../lib/SDL_ttf/lib/x64",   -- SDL_ttf libraries
    "$(SolutionDir)lib\\zlib\\contrib\\vstudio\\vc14\\x64\\ZlibStat$(Configuration)\\",
    "../lib/Vulkan/lib",        -- Local Vulkan libraries
    "../lib/Lua/lib"            -- Lua library directory
}

links
{
    "SDL2",       -- Link SDL2
    "SDL2main",   -- Link SDL2main
    "SDL2_image", -- Link SDL_image
    "SDL2_mixer", -- Link SDL_mixer
    "SDL2_ttf",   -- Link SDL_ttf
    "zlibstat",   -- Link zlib
    "vulkan-1",   -- Vulkan runtime library
    "lua54"       -- Link to the Lua import library
}

-- Post-build commands to copy DLLs to the target directory
filter "configurations:Debug or Release or Dist"
-- Post-build commands to copy DLLs to the target directory
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
