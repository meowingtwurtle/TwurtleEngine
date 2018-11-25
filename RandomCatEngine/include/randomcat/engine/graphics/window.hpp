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
        explicit window(std::string _title = "Twurtle Engine", int _width = 600, int _height = 600) noexcept;

        ~window() noexcept;

        window(window const&) = delete;
        window(window&&) = delete;

        glm::ivec2 size() const noexcept { return m_size; }

        void set_size(glm::ivec2 _size) noexcept {
            m_size = std::move(_size);
            SDL_SetWindowSize(m_window, m_size.x, m_size.y);
        }

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
        glm::ivec2 m_size;

        friend void randomcat::engine::graphics::detail::set_render_context(window const&);
    };
}    // namespace randomcat::engine::graphics
