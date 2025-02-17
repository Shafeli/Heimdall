#include "SDLWindowSystem.h"

void Brokkr::SDLWindowSystem::Render()
{
    for (const auto& sdlRenderer : m_pRenderers)
    {
        sdlRenderer->DisplayRender();
    }
}

void Brokkr::SDLWindowSystem::ClearRenders()
{
    for (const auto& sdlRenderer : m_pRenderers)
    {
        sdlRenderer->ClearRenderer();
    }
}
