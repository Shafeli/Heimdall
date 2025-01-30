#pragma once

#include <string>
#include <unordered_map>
#include "../../AssetManager/AssetManager.h"

struct SDL_Texture;

namespace Brokkr
{
    class SDLWindow;
    class SDLRenderer;
    class Texture;

    class TextureManager final : public AssetSubsystem
    {
        SDLRenderer* m_pRenderer = nullptr;
        SDLWindow* m_pWindow = nullptr;

        std::unordered_map<std::string, Texture*> m_pImageTexture;

    public:
        TextureManager(AssetManager* assetManager);
        virtual ~TextureManager() override;

        virtual void Destroy() override;

        void LoadTexture(const std::string& textureName, const char* fileName);

        void Unload(const std::string& textureName);

        [[nodiscard]] bool IsLoaded(const std::string& textureName) const;

        Texture* GetTexture(const std::string& textureName);

    protected:
        friend class CoreSystems;
        friend class AssetManager;

        void Init(SDLWindow* window, SDLRenderer* renderer);

    private:

        //Retriever
        ////////////////////////////////////////////////////
        [[nodiscard]] SDL_Texture* GetTexture(const std::string& textureName) const;  // pull texture
    };
}
