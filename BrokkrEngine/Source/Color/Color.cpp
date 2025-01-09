#include "Color.h"

#include <algorithm>
#include <cctype>
#include <SDL_pixels.h>
#include <string>
#include <unordered_map>

Brokkr::Color::Color(const std::string& colorName)
{
    m_sdlColor = std::make_unique<SDL_Color>();
    SetColor(ConvertStringToColor(colorName));
}

Brokkr::Color::~Color()
{
  //
}

Brokkr::Color::Color()
{
    m_sdlColor = std::make_unique<SDL_Color>();

    m_sdlColor->r = 0;
    m_sdlColor->g = 0;
    m_sdlColor->b = 0;
    m_sdlColor->a = 255; // Default to black
}

void Brokkr::Color::SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t opacity)
{
    m_sdlColor->r = red;
    m_sdlColor->g = green;
    m_sdlColor->b = blue;
    m_sdlColor->a = opacity;
}

Brokkr::BrokkrColor Brokkr::Color::ConvertStringToColor(const std::string& colorName) const
{
    static std::unordered_map<std::string, BrokkrColor> colorMap = {
        { "red", BrokkrColor::Red },
        { "green", BrokkrColor::Green },
        { "blue", BrokkrColor::Blue },
        { "white", BrokkrColor::White },
        { "black", BrokkrColor::Black },
        { "yellow", BrokkrColor::Yellow },
        { "cyan", BrokkrColor::Cyan },
        { "magenta", BrokkrColor::Magenta }
        // TODO: Colors Add more as needed
    };

    std::string lowerColorName = colorName;
    std::transform(lowerColorName.begin(), lowerColorName.end(), lowerColorName.begin(), ::tolower);

    auto it = colorMap.find(lowerColorName);
    if (it != colorMap.end())
    {
        return it->second;
    }
    return BrokkrColor::Debug; // Default
}

void Brokkr::Color::SetColor(BrokkrColor color)
{
    switch (color)
    {
    case BrokkrColor::Red:
        SetColor(255, 0, 0);
        break;
    case BrokkrColor::Green:
        SetColor(0, 255, 0);
        break;
    case BrokkrColor::Blue:
        SetColor(0, 0, 255);
        break;
    case BrokkrColor::White:
        SetColor(255, 255, 255);
        break;
    case BrokkrColor::Black:
        SetColor(0, 0, 0);
        break;
    case BrokkrColor::Yellow:
        SetColor(255, 255, 0);
        break;
    case BrokkrColor::Cyan:
        SetColor(0, 255, 255);
        break;
    case BrokkrColor::Magenta:
        SetColor(255, 0, 255);
        break;
    case BrokkrColor::Debug:
        SetColor(128, 128, 128); // Gray
        break;
        // TODO: Add more cases for other colors 

    default:
        SetColor(0, 0, 0); // Default to black
        break;
    }
}

void Brokkr::Color::BlendWith(const Color& other, float ratio) const
{
    m_sdlColor->r = static_cast<uint8_t>(m_sdlColor->r * (1 - ratio) + other.m_sdlColor->r * ratio);
    m_sdlColor->g = static_cast<uint8_t>(m_sdlColor->g * (1 - ratio) + other.m_sdlColor->g * ratio);
    m_sdlColor->b = static_cast<uint8_t>(m_sdlColor->b * (1 - ratio) + other.m_sdlColor->b * ratio);
    m_sdlColor->a = static_cast<uint8_t>(m_sdlColor->a * (1 - ratio) + other.m_sdlColor->a * ratio);
}

SDL_Color* Brokkr::Color::GetSdlColor() const
{
    return m_sdlColor.get();
}

void Brokkr::Color::RandomizeColors()
{
    //m_sdlColor->r = static_cast<uint8_t>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
    //m_sdlColor->g = static_cast<uint8_t>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
    //m_sdlColor->b = static_cast<uint8_t>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
    //m_sdlColor->a = static_cast<uint8_t>(g_rng.RandomRange(0, 255)); // Random opacity
}

bool Brokkr::Color::operator==(const Color& other) const
{
    return m_sdlColor->r == other.m_sdlColor->r &&
        m_sdlColor->g == other.m_sdlColor->g &&
        m_sdlColor->b == other.m_sdlColor->b &&
        m_sdlColor->a == other.m_sdlColor->a;
}
