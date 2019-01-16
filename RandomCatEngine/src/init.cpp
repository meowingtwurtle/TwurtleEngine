#include <stdexcept>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <randomcat/engine/detail/log.hpp>
#include <randomcat/engine/graphics/init.hpp>
#include <randomcat/engine/init.hpp>

namespace randomcat::engine {
    namespace {
        bool g_initDone = false;
    }

    void init() {
        if (g_initDone) {
            log::warn("Init called when already initialized!");
            return;
        }

        g_initDone = true;

        log::info("Beginning initialization...");

        if (SDL_Init(SDL_INIT_EVERYTHING)) { throw init_error{std::string{"Error initializing SDL: "} + SDL_GetError()}; }

        graphics::init();

        log::info("Successfully initialized.");
    }
}    // namespace randomcat::engine
