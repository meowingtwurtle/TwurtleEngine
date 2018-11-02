#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

#include <randomcat/engine/detail/log.h>
#include <randomcat/engine/graphics/detail/gl_context.h>
#include <randomcat/engine/graphics/window.h>
#include <randomcat/engine/init.h>

namespace randomcat::engine::graphics {
    class window {
    public:
        explicit window(std::string _title = "Twurtle Engine", int _width = 600, int _height = 600);

        ~window();

        glm::ivec2 size() const { return m_size; }

        void set_size(glm::ivec2 _size) {
            m_size = std::move(_size);
            SDL_SetWindowSize(m_window, m_size.x, m_size.y);
        }

        std::string title() const { return m_title; }

        void set_title(std::string _title) {
            m_title = std::move(_title);
            SDL_SetWindowTitle(m_window, m_title.c_str());
        }

        void swap_buffers() { SDL_GL_SwapWindow(m_window); }

        void set_cursor_shown(bool _shown) { SDL_SetRelativeMouseMode(!_shown ? SDL_TRUE : SDL_FALSE); }

    private:
        SDL_Window* m_window;
        std::string m_title;
        glm::ivec2 m_size;

        friend void randomcat::engine::graphics::detail::set_render_context(window const&);
    };
}    // namespace randomcat::engine::graphics
