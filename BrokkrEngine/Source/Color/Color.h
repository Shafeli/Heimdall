#pragma once
#include <memory>
#include <xstring>

/*
            Brokkr Color
    Blend With Sample:

    Brokkr::Color color1;
    color1.SetColor(255, 0, 0);  // Red

    Brokkr::Color color2;
    color2.SetColor(0, 0, 255);  // Blue

    color1.BlendWith(color2, 0.5); // Blend the colors with a ratio of 0.5, meaning 50% of each

    color1 Should now be purple 50/50 mix of red and blue

    Brokkr::Color color;
    color.SetColor(Brokkr::BrokkrColor::Red);  // Set color to red

    Brokkr::Color anotherColor;
    anotherColor.SetColor(Brokkr::BrokkrColor::Cyan);  // Set color to cyan
*/

struct SDL_Color;

namespace Brokkr
{
    
    enum class BrokkrColor : uint8_t
    {
        Red,
        Green,
        Blue,
        White,
        Black,
        Yellow,
        Cyan,
        Magenta,
        Debug,
        // Add from here
    };

    class Color
    {
        std::unique_ptr<SDL_Color> m_sdlColor;

    public:

        Color(const std::string& colorName);

        ~Color();

        Color();

        void RandomizeColors();   // Randomize color values
        void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t opacity = 255);
        [[nodiscard]] BrokkrColor ConvertStringToColor(const std::string& colorName) const;

        void SetColor(BrokkrColor color);

        void BlendWith(const Color& other, float ratio) const; //  blend the current Color instance with another Color instance
                                                         // by averaging their RGBA values using a given ratio

        [[nodiscard]] SDL_Color* GetSdlColor() const; // Get SDL_Color for rendering

        bool operator==(const Color& other) const;
    };
}

