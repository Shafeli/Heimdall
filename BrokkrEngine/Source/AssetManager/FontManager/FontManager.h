#pragma once
#include <SDL_ttf.h>
#include <unordered_map>

#include "../../AssetManager/AssetManager.h"


// TODO: 
struct _TTF_Font;

namespace Brokkr
{
    class WindowHandle;
    class Texture2D;
    class SDLRenderer;

    class FontManager final : public AssetSubsystem
    {
        SDLRenderer* m_pRenderer = nullptr;
        std::unordered_map<std::string, std::string> m_pFontPaths;

    public:
        explicit FontManager(AssetManager* assetManager)
            : AssetSubsystem(assetManager)
        {
            //
        }

        virtual ~FontManager() override;
        virtual void Destroy() override;


        void Load(const char* fontName, const char* filePath);
        void Unload(const char* fontName);
        [[nodiscard]] bool IsLoaded(const std::string& fontName) const;

        TTF_Font* Get(const std::string& fontName, int fontSize); // This will need to be closed after use

    protected:
        friend class CoreSystems;
        friend class AssetManager;

        void Init(SDLRenderer* renderer);
    };
}


