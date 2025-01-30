
#include "SDLWindow.h"
#include <SDL_video.h>

Brokkr::SDLWindow::~SDLWindow()
{
    Destroy();
}

bool Brokkr::SDLWindow::BuildWindow(const char* pTitle, const int width, const int height)
{

    if (m_pWindow != nullptr)
    {
        SDL_DestroyWindow(m_pWindow);
    }

    m_pWindow = SDL_CreateWindow
    (
        pTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    m_pWindowTitle = pTitle;
    m_width = width;
    m_height = height;

    if (!m_pWindow)
    {
        // "Window was not created."
        return false;
    }

    return true;
}

void Brokkr::SDLWindow::Destroy()
{
    SDL_DestroyWindow(m_pWindow);
    m_pWindow = nullptr;
    m_pWindowTitle = "Null";
    m_width = -1;
    m_height = -1;
}
