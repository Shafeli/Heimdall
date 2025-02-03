#include "TextureManager2D.h"

#include <SDL_image.h>

#include "2DRendering/SDLWindow.h"
#include "2DTexture/Texture2D.h"

Brokkr::TextureManager2D::TextureManager2D(AssetManager* assetManager)
    : AssetSubsystem(assetManager)
{
}

Brokkr::TextureManager2D::~TextureManager2D()
{
    Destroy();
}

void Brokkr::TextureManager2D::Destroy()
{
    for (const auto& [key, pTexture] : m_pImageTexture)
    {
        if (pTexture == nullptr)
            continue;

        pTexture->DestroyTexture();
        delete pTexture;
    }
}

void Brokkr::TextureManager2D::Unload(const std::string& textureName)
{
    if (IsLoaded(textureName))
    {
        //frees surface 
        m_pImageTexture[textureName]->DestroyTexture();

        // delete texture object
        delete m_pImageTexture[textureName];

        //Remove hash
        m_pImageTexture.erase(textureName);
    }
}

bool Brokkr::TextureManager2D::IsLoaded(const std::string& textureName) const
{
    //Runs over map
    for (const auto& [key, pTexture] : m_pImageTexture)
    {
        //If the key is a match to another
        if (key == textureName)
        {
            return true;
        }
    }
    return false;//No error
}

Brokkr::Texture2D* Brokkr::TextureManager2D::GetTexture(const std::string& textureName)
{
    return m_pImageTexture[textureName];
}

void Brokkr::TextureManager2D::LoadTexture(const std::string& textureName, const char* fileName)
{
    if (!IsLoaded(textureName))    // If texture is not already loaded
    {
        std::string basicString = fileName;
        m_pImageTexture[textureName] = new Texture2D(basicString, m_pRenderer);    // Add texture to map
    }
}

void Brokkr::TextureManager2D::Init(SDLWindow* window, SDLRenderer* render)
{
    m_pWindow = window;
    m_pRenderer = render;

    if (m_pWindow == nullptr)
    {
        const std::string error = "Could not set window pointer";
        
    }
    if (m_pRenderer == nullptr)
    {
        const std::string error = "Could not set renderer pointer";
       
    }
}

SDL_Texture* Brokkr::TextureManager2D::GetTexture(const std::string& textureName) const
{
    return this->m_pImageTexture.at(textureName)->LoadTexture();
}
