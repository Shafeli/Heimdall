#include "Core.h"

#include <chrono>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_events.h>

namespace Core
{
	void PrintHelloWorld()
	{
		std::cout << "Hello World!\n";
	}

}

void Brokkr::CoreSystems::UpdateDelta()
{
    SDL_Event windowEvent;
    ++frameCount;

    if (SDL_PollEvent(&windowEvent))
    {
        if (SDL_QUIT == windowEvent.type)
        {
            isRunning = false;
        }

    }

    std::chrono::time_point<std::chrono::steady_clock> thisFrameTime;
    thisFrameTime = std::chrono::high_resolution_clock::now();

    //Time between frames 
    std::chrono::duration<double> lastFrameDuration = thisFrameTime - lastFrameTime;

    //Sets the Master copy of Delta Time
    m_DeltaTime = lastFrameDuration.count();
    lastFrameTime = thisFrameTime;


    frameTimeMS += lastFrameDuration.count() * 1000.0;
    if (frameTimeMS >= 1000)
    {
        m_currentAverageFPS = frameCount;
        frameCount = 0;
        frameTimeMS -= 1000.0;
    }

}

void Brokkr::CoreSystems::Initialize()
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

    if (TTF_Init() == -1)
    {
        //m_appLogger.Log(Logger::LogLevel::kError, "SDL_TTF could not initialize! Check SDL_Error");
        return;
    }

}

void Brokkr::CoreSystems::Destroy()
{
    // Quit SDL subsystems in reverse order of initialization
    TTF_Quit();        // Quit SDL_TTF
    Mix_Quit();        // Quit SDL_mixer
    IMG_Quit();        // Quit SDL_image
    SDL_Quit();        // Quit SDL
}
