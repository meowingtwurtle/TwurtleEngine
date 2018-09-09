#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <randomcat/engine/detail/log.h>
#include <randomcat/engine/graphics/detail/gl_context.h>
#include <randomcat/engine/graphics/window.h>

namespace randomcat::engine::graphics::detail {
    namespace {
        SDL_GLContext g_context = nullptr;
    }

    void setContext(window const& _window) {
        if (g_context) {
            SDL_GL_DeleteContext(g_context);
            log::info("Destroyed previous GL context.");
        }

        g_context = SDL_GL_CreateContext(_window.m_window);

        if (!g_context) {
            log::error(std::string{"Error creating GL context: "} + SDL_GetError());
            throw context_creation_failed{};
        }

        auto glewErr = glewInit();
        if (glewErr != GLEW_OK) {
            log::error("Error initializing GLEW:" + std::string{reinterpret_cast<char const*>(glewGetErrorString(glewErr))});
            throw context_creation_failed{};
        }

        log::info("New GL context created.");
    }
}    // namespace randomcat::engine::graphics::detail
