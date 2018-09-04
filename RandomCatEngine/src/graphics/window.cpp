#include <GL/glew.h>

#include <randomcat/engine/graphics/window.h>

namespace randomcat::engine::graphics {
    struct window_creation_failed {};

    window::window(std::string _title, int _width, int _height) : m_title(std::move(_title)), m_size{_width, _height} {
        m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
        log::info("Window created.");
    }

    window::~window() {
        SDL_DestroyWindow(m_window);
        log::info("Window destroyed.");
    }
}    // namespace randomcat::engine::graphics
