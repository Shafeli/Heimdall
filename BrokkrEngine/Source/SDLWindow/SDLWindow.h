#pragma once

struct SDL_Window;

namespace Brokkr
{
    class SDLWindow
    {
        SDL_Window* m_pWindow = nullptr;
        const char* m_pWindowTitle;
        int m_width;
        int m_height;

    public:
        explicit SDLWindow(const char* pWindowTitle, int width, int height)
            : m_pWindowTitle(pWindowTitle)
            , m_width(width)
            , m_height(height)
        {
            BuildWindow(pWindowTitle, width, height);
        }

        ~SDLWindow();

        bool BuildWindow(const char* pTitle, int width, int height);

        [[nodiscard]] const char* GetTitle() const { return m_pWindowTitle; }
        [[nodiscard]] SDL_Window* GetWindow() const { return m_pWindow; }

        void Destroy();
    };

}

