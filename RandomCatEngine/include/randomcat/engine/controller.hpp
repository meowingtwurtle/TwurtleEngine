#pragma once

#include <chrono>
#include <memory>

#include <randomcat/engine/graphics/global_gl_calls.hpp>
#include <randomcat/engine/graphics/window.hpp>
#include <randomcat/engine/init.hpp>
#include <randomcat/engine/input/input_state.hpp>

namespace randomcat::engine {
    class controller {
    public:
        explicit controller(std::string _windowTitle = "Twurtle Engine", int _windowWidth = 600, int _windowHeight = 600) noexcept
        : m_window{std::move(_windowTitle), _windowWidth, _windowHeight}, m_startTime{fetch_current_raw_time()}, m_lastTickTime{m_startTime}, m_currentTickTime{m_startTime} {
            graphics::gl_detail::set_render_context(m_window);
            graphics::enable_depth_test();
        }

        controller(controller const&) = delete;
        controller(controller&&) = delete;

        void tick() noexcept {
            fetch_sdl_events();

            m_lastTickTime = std::move(m_currentTickTime);
            m_currentTickTime = fetch_current_raw_time();

            graphics::clear_graphics();
        }

        input::input_state inputs() const noexcept { return m_currentInputState; }

        std::chrono::milliseconds start_time() const noexcept { return m_startTime; }

        std::chrono::milliseconds previous_time() const noexcept { return m_lastTickTime; }

        std::chrono::milliseconds current_time() const noexcept { return m_currentTickTime; }

        std::chrono::milliseconds delta_time() const noexcept { return current_time() - previous_time(); }

        template<typename _renderer_t, typename... _renderer_arg_t>
        void render(_renderer_t const& _renderer,
                    _renderer_arg_t&&... _rendererArg) noexcept(noexcept(_renderer(std::forward<_renderer_arg_t>(_rendererArg)...))) {
            _renderer(std::forward<_renderer_arg_t>(_rendererArg)...);
            m_window.swap_buffers();
        }

        graphics::window& window() noexcept { return m_window; }
        graphics::window const& window() const noexcept { return m_window; }

        bool quit_received() const noexcept { return m_quitReceived; }

    private:
        graphics::window m_window;

        std::chrono::milliseconds m_startTime;
        std::chrono::milliseconds m_lastTickTime;
        std::chrono::milliseconds m_currentTickTime;

        input::input_state m_currentInputState;

        bool m_quitReceived = false;

        std::chrono::milliseconds fetch_current_raw_time() const noexcept { return std::chrono::milliseconds{SDL_GetTicks()}; }

        void fetch_sdl_events() noexcept {
            input::input_state workState = std::move(m_currentInputState);

            workState.rel_mouse_x() = 0;
            workState.rel_mouse_y() = 0;

            workState.down_to_held();

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_KEYDOWN: {
                        workState.set_key_down(event.key.keysym.sym);

                        break;
                    }

                    case SDL_KEYUP: {
                        workState.set_key_up(event.key.keysym.sym);

                        break;
                    }

                    case SDL_MOUSEMOTION: {
                        workState.mouse_x() = event.motion.x;
                        workState.mouse_y() = event.motion.y;

                        workState.rel_mouse_x() += event.motion.xrel;
                        workState.rel_mouse_y() += event.motion.yrel;

                        break;
                    }

                    case SDL_QUIT:
                    case SDL_WINDOWEVENT_CLOSE: {
                        m_quitReceived = true;
                    }
                }
            }

            m_currentInputState = std::move(workState);
        }
    };
}    // namespace randomcat::engine
