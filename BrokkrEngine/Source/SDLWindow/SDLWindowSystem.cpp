#include "SDLWindowSystem.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


void SDLWindowSystem::Init()
{
    // attempt to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        return;
    }

    if (Mix_Init(MIX_INIT_MP3) < 0)
    {
        return;
    }

}

void SDLWindowSystem::Destroy()
{
    for (const auto& window : m_pWindows)
    {
        window->Destroy();
    }

    m_pWindows.clear();

    // Quit SDL subsystems in reverse order of initialization
    Mix_Quit();        // Quit SDL_mixer
    IMG_Quit();        // Quit SDL_image
    SDL_Quit();        // Quit SDL
}
