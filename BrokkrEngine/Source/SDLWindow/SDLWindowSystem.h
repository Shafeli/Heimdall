#pragma once

#include <algorithm>
#include <cstring>
#include <memory>
#include <type_traits>

#include "SDLRenderer.h"
#include "Core/Core.h"
#include "SDLWindow/SDLWindow.h"

// TODO: Edge cases possible issues SDLWindowManagement system
/*
    * Adding auxiliary renderers for a window would break index and window organization

*/
namespace Brokkr
{
    class SDLRenderer;

    class SDLWindowSystem final : public Brokkr::System
    {
        std::vector<std::unique_ptr<SDLWindow>> m_pWindows;
        std::vector<std::unique_ptr<SDLRenderer>> m_pRenderers;

    public:
        explicit SDLWindowSystem(CoreSystems* pCoreManager)
            : System(pCoreManager)
        {
            //
        }

        // Get window by name
        template <typename SDLWindow>
        SDLWindow* GetWindowByName(const char* pWindowTitle)
        {
            for (auto& pWindow : m_pWindows)
            {
                if (Brokkr::SDLWindow* target = pWindow.get())
                {
                    if (std::strcmp(target->GetTitle(), pWindowTitle) == 0)
                        return target;
                }
            }
            return nullptr;
        }

        // Add a new window
        template <typename SDLWindow, typename... Args>
        SDLWindow* AddWindow(Args&&... args)
        {
            static_assert(std::is_base_of_v<Brokkr::SDLWindow, SDLWindow>,
                "Window must derive from Brokkr::SDLWindow");

            auto newWindow = std::make_unique<SDLWindow>(std::forward<Args>(args)...);
            SDLWindow* result = newWindow.get();
            m_pWindows.emplace_back(std::move(newWindow));

            if (result)
            {
                // Create a default renderer tied to this window
                AddRenderer<SDLRenderer>(result);
            }

            return result;
        }

        // Remove a window and its renderer
        template <typename SDLWindow>
        void RemoveWindow(SDLWindow* window)
        {
            static_assert(std::is_base_of_v<Brokkr::SDLWindow, SDLWindow>,
                "Window must derive from Brokkr::SDLWindow");

            const auto findWindow = std::find_if
            (
                m_pWindows.begin(),
                m_pWindows.end(),
                [window](const std::unique_ptr<SDLWindow>& currentWindow) {return currentWindow.get() == window; }
            );

            if (findWindow != m_pWindows.end())
            {
                const size_t index = std::distance(m_pWindows.begin(), findWindow);
                RemoveRenderer(index); // Remove associated renderer by index
                (*findWindow)->Destroy();
                m_pWindows.erase(findWindow);
            }
        }

        // Get renderer associated with a specific window
        SDLRenderer* GetRendererForWindow(SDLWindow* window) const
        {
            const auto findWindow = std::find_if
            (
                m_pWindows.begin(),
                m_pWindows.end(),
                [window](const std::unique_ptr<SDLWindow>& currentWindow) {return currentWindow.get() == window; }
            );

            if (findWindow != m_pWindows.end())
            {
                size_t index = std::distance(m_pWindows.begin(), findWindow);
                return GetRenderer(index);
            }

            return nullptr;
        }

        virtual void Destroy() override
        {
            // Destroy renderers first
            for (auto& renderer : m_pRenderers)
            {
                renderer->Destroy();
            }
            m_pRenderers.clear();

            // Then destroy windows
            for (auto& window : m_pWindows)
            {
                window->Destroy();
            }
            m_pWindows.clear();
        }

    private:

        // Add a new renderer for a window
        template <typename SDLRenderer, typename... Args>
        SDLRenderer* AddRenderer(SDLWindow* window, Args&&... args)
        {
            static_assert(std::is_base_of_v<Brokkr::SDLRenderer, SDLRenderer>,
                "Renderer must derive from Brokkr::SDLRenderer");

            auto newRenderer = std::make_unique<SDLRenderer>(window, std::forward<Args>(args)...);
            SDLRenderer* result = newRenderer.get();
            m_pRenderers.emplace_back(std::move(newRenderer));

            return result;
        }

        // Get a renderer by its index
        SDLRenderer* GetRenderer(size_t index) const
        {
            if (index < m_pRenderers.size())
            {
                return m_pRenderers[index].get();
            }
            return nullptr;
        }

        // Remove a renderer by index
        void RemoveRenderer(size_t index)
        {
            if (index < m_pRenderers.size())
            {
                m_pRenderers[index]->Destroy();
                m_pRenderers.erase(m_pRenderers.begin() + index);
            }
        }

    };

}
