#pragma once
#include "Color/Color.h"
#include "Primitives/Rect.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace Brokkr
{
    class SDLWindow;
    class BasicColor;

    class SDLRenderer
    {
        SDL_Renderer* m_pRenderer = nullptr;
        SDLWindow* m_pWindow = nullptr;

    public:
        explicit SDLRenderer(SDLWindow* pWindow)
            :m_pWindow(pWindow)
        {
            BuildRenderer();
        }

        ~SDLRenderer();

        void BuildRenderer();
        void ClearRenderer() const;

        void RenderCopy(SDL_Texture* texture, int x, int y, int w, int h) const;

        void RenderCopy(SDL_Texture* texture, Rect<int> transform, Rect<int> sourceTransform) const;

        void Render() const;

        void SetRenderDrawColor(const Color* pColor) const;

        void RenderCircle(int centerX, int centerY, int radius, int red, int green, int blue) const;
        void RenderSquare(int x, int y, int h, int w, int red, int green, int blue, int opacity) const;
        void RenderLine(int x1, int y1, int x2, int y2, int red, int green, int blue) const;

        [[nodiscard]] SDL_Renderer* GetRenderer() const { return m_pRenderer; }

        void Destroy();
    };
}