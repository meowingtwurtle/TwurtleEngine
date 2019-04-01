#include "randomcat/engine/graphics/render_context.hpp"

#include <GL/glew.h>

namespace randomcat::engine::graphics {
    render_context::render_context(window const& _window) noexcept(false)
    : m_context{_window.raw_pointer(impl_call), SDL_GL_CreateContext(_window.raw_pointer(impl_call))} {
        auto l = make_active_lock();
        enable_depth_test();

        auto glewErr = glewInit();
        if (glewErr != GLEW_OK) {
            throw render_context_init_error{"Error initializing GLEW: " + std::string{reinterpret_cast<char const*>(glewGetErrorString(glewErr))}};
        }
    }
}    // namespace randomcat::engine::graphics
