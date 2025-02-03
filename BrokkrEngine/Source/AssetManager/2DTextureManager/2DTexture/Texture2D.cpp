#include "Texture2D.h"
#include <SDL.h>
#include <SDL_image.h>
#include "2DRendering/SDLRenderer.h"

Brokkr::Texture2D::Texture2D(const std::string& texturePath, SDLRenderer* pRenderer)
    :m_texturePath(texturePath)
    ,m_pRenderer(pRenderer)
{
    LoadTexture();
}

Brokkr::Texture2D::Texture2D(const void* pData, size_t size, SDLRenderer* pRenderer)
    :m_texturePath("BinaryBuilt" + size)
    ,m_pRenderer(pRenderer)
{
    SDL_RWops* pRWops = SDL_RWFromConstMem(pData, static_cast<int>(size));

    // SDL_Surface object
    SDL_Surface* pSurface = IMG_Load_RW(pRWops, 0);

    m_size.x = pSurface->w;
    m_size.y = pSurface->h;

    // Create a surface
    m_pTexture = SDL_CreateTextureFromSurface(pRenderer->GetRenderer(), pSurface);

    // Free the surface and RWops
    SDL_FreeSurface(pSurface);
    SDL_RWclose(pRWops);
}

Brokkr::Texture2D::~Texture2D()
{
    DestroyTexture();
}

void Brokkr::Texture2D::SetTextureColor(Color newModColor) const
{
    auto newColor = newModColor.GetSdlColor();

    // Modulate the texture's RGB values
    SDL_SetTextureColorMod(m_pTexture, newColor->r, newColor->g, newColor->b);
}

void Brokkr::Texture2D::ResetTextureColor() const
{
    // Reset the texture color
    SDL_SetTextureColorMod(m_pTexture, 255, 255, 255);
}

SDL_Texture* Brokkr::Texture2D::LoadTexture()
{
    if (m_pTexture)
        return m_pTexture;

    if (m_pTexture == nullptr)    // If texture is not already loaded
    {
        SDL_Surface* pSurface = IMG_Load(m_texturePath.c_str()); // Load the image
        if (pSurface == nullptr)
        {
            // const std::string error = "Could not load surface : " + m_texturePath + ", error message: " + IMG_GetError();
            return nullptr;
        }

        // Create texture from surface
        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pRenderer->GetRenderer(), pSurface);
        if (pTexture == nullptr)
        {
            // const std::string error = "Could not load texture : " + m_texturePath + " " + SDL_GetError();
            SDL_FreeSurface(pSurface);

            return nullptr;
        }

        m_pTexture = pTexture;    // Add texture to map

        m_size.x = pSurface->w;
        m_size.y = pSurface->h;

        SDL_FreeSurface(pSurface);  // Free the surface
    }

    return m_pTexture;
}

bool Brokkr::Texture2D::DestroyTexture()
{
    if (m_pTexture != nullptr)
    {
        SDL_DestroyTexture(m_pTexture);
        return true;
    }

    return false; 
}
