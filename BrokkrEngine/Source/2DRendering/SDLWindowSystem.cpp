#include "SDLWindowSystem.h"
#include "3DRendering/VulkanRenderer.h"

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

template <typename SDLRenderer, typename ... Args>
SDLRenderer* Brokkr::SDLWindowSystem::AddRenderer(SDLWindow* window, Args&&... args)
{
    static_assert(std::is_base_of_v<Brokkr::SDLRenderer, SDLRenderer>,
        "Renderer must derive from Brokkr::SDLRenderer");

    auto newRenderer = std::make_unique<SDLRenderer>(window, std::forward<Args>(args)...);
    SDLRenderer* result = newRenderer.get();
    m_pRenderers.emplace_back(std::move(newRenderer));

    return result;
}
