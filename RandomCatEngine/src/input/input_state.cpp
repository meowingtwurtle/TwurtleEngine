#include <iostream>

#include <randomcat/engine/input/input_state.h>

namespace randomcat::engine::input {
    void input_state::update() noexcept {
        m_relMouseX = 0;
        m_relMouseY = 0;

        down_to_held();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    set_key_down(event.key.keysym.sym);

                    break;
                }

                case SDL_KEYUP: {
                    set_key_up(event.key.keysym.sym);

                    break;
                }

                case SDL_MOUSEMOTION: {
                    m_mouseX = event.motion.x;
                    m_mouseY = event.motion.y;

                    m_relMouseX += event.motion.xrel;
                    m_relMouseY += event.motion.yrel;

                    break;
                }

                case SDL_QUIT:
                case SDL_WINDOWEVENT_CLOSE: {
                    m_shouldQuit = true;
                }
            }
        }
    }
}    // namespace randomcat::engine::input
