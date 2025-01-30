#pragma once


namespace Brokkr
{
    struct EngineDefinitions
    {
        // File Paths
        inline static const char* CONFIG_FILE = "Config/SystemConfig.xml";
        inline static const char* ASSETS_PATH = "Assets/";

        // Event System
        inline static const char* UPDATE_EVENT = "Update";
        inline static const char* UPDATE_TRANSFORM_POSITION_EVENT = "UpdatePosition";

        inline static int MAX_EVENTS = 256; // No need for constexpr here

        // Math
        static constexpr double PI = 3.141592653589793;
        static constexpr double DEG_TO_RAD = PI / 180.0;
        static constexpr double RAD_TO_DEG = 180.0 / PI;
    };

    struct EngineLimits
    {
        static constexpr int MAX_TEXTURES = 512;
    };

}

