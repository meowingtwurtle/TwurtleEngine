#include "randomcat/engine/graphics/window.hpp"

#include <SDL2/SDL.h>

namespace randomcat::engine::graphics {
    window::window(std::string const& _title, int16_t _width, int16_t _height) noexcept {
        m_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);

        log::info << "Window created with title \"" << _title << "\".";
    }

    window::~window() noexcept {
        log::info << "Window with title \"" << title() << "\" destroyed.";
        SDL_DestroyWindow(m_window);
    }

    void window::set_size(dimensions _dims) noexcept { SDL_SetWindowSize(m_window, _dims.width, _dims.height); }

    window::dimensions window::size() const noexcept {
        // SDL requires use of int
        int width, height;
        SDL_GetWindowSize(m_window, &width, &height);

        return dimensions{.width = gsl::narrow<std::int16_t>(width), .height = gsl::narrow<std::int16_t>(height)};
    }

    std::string window::title() const noexcept { return SDL_GetWindowTitle(m_window); }

    void window::set_title(std::string const& _title) noexcept { SDL_SetWindowTitle(m_window, _title.c_str()); }

    void window::set_cursor_shown(bool _shown) noexcept { SDL_SetRelativeMouseMode(!_shown ? SDL_TRUE : SDL_FALSE); }
}    // namespace randomcat::engine::graphics
