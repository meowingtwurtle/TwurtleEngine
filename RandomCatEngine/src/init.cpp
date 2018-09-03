#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <twurtle/init.h>
#include <twurtle/log.h>
#include <twurtle/window.h>

namespace randomcat::graphics {
    namespace {
        bool g_initDone = false;
    }

    struct init_failed {};

    void init() {
        if (g_initDone) {
            log::warn("Init called when already initialized!");
            return;
        }

        g_initDone = true;

        log::info("Beginning initialization...");

        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            log::error(std::string{"Error initializing SDL: "} + SDL_GetError());
            throw init_failed{};
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        log::info("Successfully initialized.");
    }

    namespace {
        SDL_GLContext g_context = nullptr;
    }

    struct context_creation_failed {};

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
}    // namespace randomcat::graphics
