#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <gsl/gsl_util>

#include "randomcat/engine/detail/log.hpp"
#include "randomcat/engine/graphics/detail/gl_context.hpp"
#include "randomcat/engine/graphics/window.hpp"
#include "randomcat/engine/init.hpp"

namespace randomcat::engine::graphics {
    class window {
    public:
        explicit window(std::string const& _title = "Twurtle Engine", std::int16_t _width = 600, std::int16_t _height = 600) noexcept {
            m_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);

            log::info << "Window created with title \"" << _title << "\".";
        }

        ~window() noexcept {
            log::info << "Window with title \"" << title() << "\" destroyed.";
            SDL_DestroyWindow(m_window);
        }

        window(window const&) = delete;
        window(window&&) = delete;

        // Use of raw int required by SDL
        void set_size(int _width, int _height) noexcept { SDL_SetWindowSize(m_window, _width, _height); }

        auto width() const noexcept { return size().width; }

        auto height() const noexcept { return size().height; }

        struct dimensions {
            std::int16_t width;
            std::int16_t height;
        };

        dimensions size() const noexcept {
            // SDL requires use of int
            int width, height;
            SDL_GetWindowSize(m_window, &width, &height);

            return {.width = gsl::narrow<std::int16_t>(width), .height = gsl::narrow<std::int16_t>(height)};
        }

        float aspect_ratio() const noexcept {
            dimensions dims = size();
            return float(dims.width) / float(dims.height);
        }

        std::string title() const noexcept { return SDL_GetWindowTitle(m_window); }

        void set_title(std::string const& _title) noexcept { SDL_SetWindowTitle(m_window, _title.c_str()); }

        void swap_buffers() noexcept { SDL_GL_SwapWindow(m_window); }

        void set_cursor_shown(bool _shown) noexcept { SDL_SetRelativeMouseMode(!_shown ? SDL_TRUE : SDL_FALSE); }

    private:
        SDL_Window* m_window;

        friend void randomcat::engine::graphics::gl_detail::set_render_context(window const&);
    };
}    // namespace randomcat::engine::graphics
