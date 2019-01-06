#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

#include <randomcat/engine/detail/log.hpp>
#include <randomcat/engine/graphics/detail/gl_context.hpp>
#include <randomcat/engine/graphics/window.hpp>
#include <randomcat/engine/init.hpp>

namespace randomcat::engine::graphics {
    class window {
    public:
        explicit window(std::string _title = "Twurtle Engine", int _width = 600, int _height = 600) noexcept
        : m_title(std::move(_title)), m_width{_width}, m_height{_height} {
            m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);

            log::info << "Window created with title \"" << m_title << "\".";
        }

        ~window() noexcept {
            SDL_DestroyWindow(m_window);
            log::info << "Window with title \"" << m_title << "\" destroyed.";
        }

        window(window const&) = delete;
        window(window&&) = delete;

        void set_size(int _width, int _height) noexcept {
            m_width = _width;
            m_height = _height;
            SDL_SetWindowSize(m_window, m_width, m_height);
        }

        int width() const noexcept { return m_width; }
        int height() const noexcept { return m_height; }

        std::string title() const noexcept { return m_title; }

        void set_title(std::string _title) noexcept {
            m_title = std::move(_title);
            SDL_SetWindowTitle(m_window, m_title.c_str());
        }

        void swap_buffers() noexcept { SDL_GL_SwapWindow(m_window); }

        void set_cursor_shown(bool _shown) noexcept { SDL_SetRelativeMouseMode(!_shown ? SDL_TRUE : SDL_FALSE); }

    private:
        SDL_Window* m_window;
        std::string m_title;
        int m_width;
        int m_height;

        friend void randomcat::engine::graphics::detail::set_render_context(window const&);
    };
}    // namespace randomcat::engine::graphics
