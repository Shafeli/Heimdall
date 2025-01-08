#include "Core.h"

#include <chrono>
#include <iostream>
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


    frameTime += lastFrameDuration.count() * 1000.0;
    if (frameTime >= 1000)
    {
        m_currentFPS = frameCount;
        frameCount = 0;
        frameTime -= 1000.0;
    }

}
