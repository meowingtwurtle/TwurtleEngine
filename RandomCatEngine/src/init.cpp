#include <SDL2/SDL.h>
#include <twurtle/init.h>
#include <twurtle/log.h>

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
}    // namespace randomcat::graphics
