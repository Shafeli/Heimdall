#pragma once

#include <string>
#include <unordered_map>
#include "../../AssetManager/AssetManager.h"

struct SDL_Texture;

namespace Brokkr
{
    class SDLWindow;
    class SDLRenderer;
    class Texture2D;

    class TextureManager2D final : public AssetSubsystem
    {
        SDLRenderer* m_pRenderer = nullptr;
        SDLWindow* m_pWindow = nullptr;

        std::unordered_map<std::string, Texture2D*> m_pImageTexture;

    public:
        TextureManager2D(AssetManager* assetManager);
        virtual ~TextureManager2D() override;

        virtual void Destroy() override;

        void LoadTexture(const std::string& textureName, const char* fileName);

        void Unload(const std::string& textureName);

        [[nodiscard]] bool IsLoaded(const std::string& textureName) const;

        Texture2D* GetTexture(const std::string& textureName);

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
