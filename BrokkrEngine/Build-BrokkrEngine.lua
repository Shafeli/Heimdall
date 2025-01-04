project "BrokkrEngine"
kind "StaticLib"
language "C++"
cppdialect "C++17"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"

files { "Source/**.h", "Source/**.cpp" }

includedirs
{
    "Source",                   -- Your source directory
    "../lib/SDL2/include",      -- SDL2 headers
    "../lib/SDL_image/include", -- SDL_image headers
    "../lib/SDL_mixer/include", -- SDL_mixer headers
    "../lib/SDL_ttf/include",   -- SDL_ttf headers
    "../lib/zlib",              -- zlib headers
}

libdirs
{
    "../lib/SDL2/lib/x64",      -- SDL2 libraries (e.g., SDL2.lib)
    "../lib/SDL_image/lib/x64", -- SDL_image libraries (e.g., SDL2_image.lib)
    "../lib/SDL_mixer/lib/x64", -- SDL_mixer libraries (e.g., SDL2_mixer.lib)
    "../lib/SDL_ttf/lib/x64",   -- SDL_ttf libraries (e.g., SDL2_ttf.lib)
    path.join(SolutionDir, "lib", "zlib", "contrib", "vstudio", "vc14", _ACTION == "x64" and "x64" or "x86", "ZlibStat", _OPTIONS["config"]),

}

links
{
    "SDL2",       -- Link SDL2 (SDL2.lib)
    "SDL2main",   -- Link SDL2main (SDL2main.lib)
    "SDL2_image", -- Link SDL_image (SDL2_image.lib)
    "SDL2_mixer", -- Link SDL_mixer (SDL2_mixer.lib)
    "SDL2_ttf",   -- Link SDL_ttf (SDL2_ttf.lib)
    "zlibstat",   -- Link zlib (zlib.lib)
}

-- Post-build commands to copy DLLs to the target directory
filter "configurations:Debug or Release or Dist"
postbuildcommands
{
    "{COPY} lib/SDL2/bin/SDL2.dll %{cfg.targetdir}/",
    "{COPY} lib/SDL_image/bin/SDL2_image.dll %{cfg.targetdir}/",
    "{COPY} lib/SDL_mixer/bin/SDL2_mixer.dll %{cfg.targetdir}/",
    "{COPY} lib/SDL_ttf/bin/SDL2_ttf.dll %{cfg.targetdir}/",
    "{COPY} lib/zlib/bin/zlibstat.dll %{cfg.targetdir}/"
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
