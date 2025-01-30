#include "FontManager.h"

#include <SDL_ttf.h>

Brokkr::FontManager::~FontManager()
{
    //
}

void Brokkr::FontManager::Destroy()
{
    //
}

void Brokkr::FontManager::Load(const char* fontName, const char* filePath)
{
    if (!IsLoaded(fontName))
    {
        m_pFontPaths.emplace(fontName, filePath);
    }
}

void Brokkr::FontManager::Unload(const char* fontName)
{
    if (IsLoaded(fontName))
    {
        m_pFontPaths.erase(fontName);
    }
}


bool Brokkr::FontManager::IsLoaded(const std::string& fontName) const
{
    return m_pFontPaths.find(fontName) != m_pFontPaths.end();
}

TTF_Font* Brokkr::FontManager::Get(const std::string& fontName, int fontSize)
{

    // Load the font if it's in paths
    const auto fontPathIt = m_pFontPaths.find(fontName);
    if (fontPathIt != m_pFontPaths.end())
    {
        TTF_Font* font = TTF_OpenFont(fontPathIt->second.c_str(), fontSize);
        if (font)
        {
            return font;
        }
        //m_fileLog.Log(Logger::LogLevel::kError, "Failed to load font: " + fontName + " Error: " + TTF_GetError());
    }
    else
    {
        //m_fileLog.Log(Logger::LogLevel::kWarning, "Font path not found for: " + fontName);
    }

    return nullptr;
}

void Brokkr::FontManager::Init(SDLRenderer* renderer)
{
    m_pRenderer = renderer;
    if (!m_pRenderer)
    {
        //m_fileLog.Log(Logger::LogLevel::kError, "Renderer is Null When Initializing Font Manager System");
    }
}