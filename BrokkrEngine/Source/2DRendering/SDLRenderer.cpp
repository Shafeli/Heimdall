#include "SDLRenderer.h"

#include <SDL_render.h>
#include "SDLWindow.h"
#include "Color/Color.h"

Brokkr::SDLRenderer::~SDLRenderer()
{
    Destroy();
}

void Brokkr::SDLRenderer::BuildRenderer()
{
    m_pRenderer = SDL_CreateRenderer(m_pWindow->GetWindow(), -1, SDL_RENDERER_ACCELERATED);
}

void Brokkr::SDLRenderer::ClearRenderer() const
{
    SDL_RenderClear(m_pRenderer);
}

void Brokkr::SDLRenderer::RenderCopy(SDL_Texture* texture, int x, int y, int w, int h) const
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    M_PI;

    SDL_RenderCopy(m_pRenderer, texture, nullptr, &destination);
}

void Brokkr::SDLRenderer::RenderCopy(SDL_Texture* texture, Rectangle<int> transform, Rectangle<int> sourceTransform) const
{
    SDL_Rect destination;
    destination.x = transform.GetX();
    destination.y = transform.GetY();
    destination.w = transform.GetWidth();
    destination.h = transform.GetHeight();

    SDL_Rect source;
    source.x = sourceTransform.GetX();
    source.y = sourceTransform.GetY();
    source.w = sourceTransform.GetWidth();
    source.h = sourceTransform.GetHeight();

    SDL_RenderCopy(m_pRenderer, texture, &source, &destination);
}

void Brokkr::SDLRenderer::DisplayRender() const
{
    SDL_RenderPresent(m_pRenderer);
}

void Brokkr::SDLRenderer::SetRenderDrawColor(const Color* pColor) const
{
    const auto sdlColor = pColor->GetSdlColor();

    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        sdlColor->r,
        sdlColor->g,
        sdlColor->b,
        sdlColor->a
    );
}

void Brokkr::SDLRenderer::RenderCircle(const Circle<int>& pCircle, const Color& pColor) const
{
    RenderCircle(
        pCircle.GetCenter().x,
        pCircle.GetCenter().y,
        pCircle.GetRadius(),
        pColor.GetSdlColor()->r,
        pColor.GetSdlColor()->g,
        pColor.GetSdlColor()->b
    );
}

void Brokkr::SDLRenderer::RenderSquareFilled(const Rectangle<int>& rectangle, const Color& color) const
{
    RenderSquareFilled(
        rectangle.GetX(),
        rectangle.GetY(),
        rectangle.GetHeight(),
        rectangle.GetWidth(),
        color.GetSdlColor()->r,
        color.GetSdlColor()->g,
        color.GetSdlColor()->b,
        color.GetSdlColor()->a)
    ;
}

void Brokkr::SDLRenderer::RenderLine(const Line<int>& line, const Color& color) const
{
    const Vector2<int> startPoint = line.GetStart();
    const Vector2<int> endPoint = line.GetEnd();

    RenderLine
    (
        startPoint.x, 
        startPoint.y, 
        endPoint.x, 
        endPoint.y, 
        color.GetSdlColor()->r,
        color.GetSdlColor()->g,
        color.GetSdlColor()->b
    );
}

void Brokkr::SDLRenderer::RenderCircle(int centerX, int centerY, int radius, int red, int green, int blue) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        SDL_ALPHA_OPAQUE
    );

    // the midpoint circle algorithm
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm has the explanation
    int x = radius;
    int y = 0;
    int distance = 0;

    while (x >= y)
    {
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY - y);

        if (distance <= 0)
        {
            y += 1;
            distance += 2 * y + 1;
        }

        if (distance > 0)
        {
            x -= 1;
            distance -= 2 * x + 1;
        }
    }
}

void Brokkr::SDLRenderer::RenderSquareFilled(int x, int y, int h, int w, int red, int green, int blue, int opacity) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        static_cast<Uint8>(opacity)
    );

    SDL_Rect rect;
    rect.h = h;
    rect.w = w;
    rect.x = x;
    rect.y = y;

    SDL_RenderFillRect(m_pRenderer, &rect);
    // SDL_RenderDrawRect(m_pRenderer, &rect);
}

void Brokkr::SDLRenderer::RenderLine(int x1, int y1, int x2, int y2, int red, int green, int blue) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        SDL_ALPHA_OPAQUE
    );

    SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void Brokkr::SDLRenderer::Destroy()
{
    if (m_pRenderer != nullptr)
        SDL_DestroyRenderer(m_pRenderer);
}
