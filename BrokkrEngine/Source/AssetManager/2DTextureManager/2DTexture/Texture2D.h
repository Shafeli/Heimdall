#pragma once

#include <string>
#include "Vector2.h"
#include "Color/Color.h"

//TODO: Add decompressed binary to Texture support

struct SDL_Texture;

namespace Brokkr
{
    class SDLRenderer;

    class Texture2D
    {

        Vector2<int> m_size;
        std::string m_texturePath;
        SDL_Texture* m_pTexture = nullptr;
        SDLRenderer* m_pRenderer = nullptr;

    public:

        Texture2D(const std::string& texturePath, SDLRenderer* pRenderer);
        Texture2D(const void* pData, size_t size, SDLRenderer* pRenderer);
        ~Texture2D();

        // Function to change the color of a texture
        void SetTextureColor(Color newModColor) const;
        void ResetTextureColor() const;
        [[nodiscard]] Vector2<int> GetSize() const { return m_size; }

        SDL_Texture* LoadTexture();
        bool DestroyTexture();
    };

}
