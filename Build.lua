workspace "Heimdall"
architecture "x64"
configurations { "Debug", "Release", "Dist" }
startproject "EitirGame"

-- Workspace-wide build options for MSVC
filter "system:windows"
buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
include "BrokkrEngine/Build-BrokkrEngine.lua"
group ""

group "Applications"
include "EitirGame/Build-EitirGame.lua"
include "ValkyrieEditor/Build-ValkyrieEditor.lua"
group ""
