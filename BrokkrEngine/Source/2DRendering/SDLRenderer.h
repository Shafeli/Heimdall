#pragma once
#include "Circle.h"
#include "Line.h"
#include "Rectangle.h"
#include "Color/Color.h"

struct SDL_Renderer;
struct SDL_Texture;

// TODO: Update Rendering Shapes to use Engine Primitives & color
// TODO: Queuing to renderer with depth sorting
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
        void RenderCopy(SDL_Texture* texture, Rectangle<int> transform, Rectangle<int> sourceTransform) const;

        void DisplayRender() const;

        void SetRenderDrawColor(const Color* pColor) const;

        void RenderCircle(const Circle<int>& pCircle, const Color& pColor) const;
        void RenderSquareFilled(const Rectangle<int>& rectangle, const Color& color) const;
        void RenderLine(const Line<int>& line, const Color& color) const;

        void RenderCircle(int centerX, int centerY, int radius, int red, int green, int blue) const;
        void RenderSquareFilled(int x, int y, int h, int w, int red, int green, int blue, int opacity) const;
        void RenderLine(int x1, int y1, int x2, int y2, int red, int green, int blue) const;

        [[nodiscard]] SDL_Renderer* GetRenderer() const { return m_pRenderer; }

        void Destroy();
    };
}