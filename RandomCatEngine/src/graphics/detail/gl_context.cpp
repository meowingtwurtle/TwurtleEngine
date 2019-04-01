#include "randomcat/engine/graphics/detail/gl_context.hpp"

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "randomcat/engine/detail/log.hpp"
#include "randomcat/engine/graphics/window.hpp"

namespace randomcat::engine::graphics::gl_detail {
    namespace {
        SDL_GLContext g_context = nullptr;
    }

    void set_render_context(window const& _window) {
        if (g_context) {
            SDL_GL_DeleteContext(g_context);
            log::info("Destroyed previous GL context.");
        }

        g_context = SDL_GL_CreateContext(_window.raw_pointer(impl_call));

        if (!g_context) { throw render_context_init_error{std::string{"Error creating GL context: "} + SDL_GetError()}; }

        auto glewErr = glewInit();
        if (glewErr != GLEW_OK) {
            throw render_context_init_error{"Error initializing GLEW: " + std::string{reinterpret_cast<char const*>(glewGetErrorString(glewErr))}};
        }

        log::info("New GL context created.");
    }
}    // namespace randomcat::engine::graphics::gl_detail
