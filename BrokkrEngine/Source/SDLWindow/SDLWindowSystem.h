#pragma once

#include <algorithm>
#include <cstring>
#include <memory>
#include <type_traits>
#include "Core/Core.h"
#include "SDLWindow/SDLWindow.h"

class SDLWindowSystem final : public Brokkr::System
{

    std::vector<std::unique_ptr<Brokkr::SDLWindow>> m_pWindows;

public:
    explicit SDLWindowSystem(Brokkr::CoreSystems* pCoreManager)
        : System(pCoreManager)
    {
        Init();
    }

    void Init();

    template <typename SDLWindow>
    SDLWindow* GetWindowByName(const char* pWindowTitle)
    {
        for (auto& pWindow : m_pWindows)
        {
            // If the cast is successful
            if (Brokkr::SDLWindow* target = pWindow.get())
            {
                if (std::strcmp(target->GetTitle(), pWindowTitle) == 0)
                    return target;
            }
        }

        return nullptr; // If no window is found
    }

    template <typename SDLWindow, typename ... Args>
    SDLWindow* AddWindow(Args&&... args)
    {
        static_assert(std::is_base_of_v<Brokkr::SDLWindow, SDLWindow>,
            "SDLWindow must derive from Brokkr::SDLWindow");

        auto newWindow = std::make_unique<SDLWindow>(std::forward<Args>(args)...);
        SDLWindow* result = newWindow.get();
        m_pWindows.emplace_back(std::move(newWindow));

        if (!result)
        {
            return nullptr;
        }

        return result; // Return a pointer
    }

    template<typename SDLWindow>
    void RemoveWindow(SDLWindow* window)
    {
        static_assert(std::is_base_of_v<Brokkr::SDLWindow, SDLWindow>,
            "SDLWindow must derive from Brokkr::SDLWindow");

        auto it = std::find_if(m_pWindows.begin(), m_pWindows.end(),
            [window](const std::unique_ptr<Brokkr::SDLWindow>& currentWindow)
            {
                return currentWindow.get() == window;
            });

        if (it != m_pWindows.end())
        {
            (*it)->Destroy();
            m_pWindows.erase(it);
        }
    }

    virtual void Destroy() override;
};

