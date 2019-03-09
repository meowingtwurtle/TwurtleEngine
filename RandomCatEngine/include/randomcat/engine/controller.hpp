#pragma once

#include <chrono>
#include <memory>

#include "randomcat/engine/controller_timer.hpp"
#include "randomcat/engine/graphics/global_gl_calls.hpp"
#include "randomcat/engine/graphics/window.hpp"
#include "randomcat/engine/init.hpp"
#include "randomcat/engine/input/input_state.hpp"

namespace randomcat::engine {
    class controller {
    public:
        explicit controller(std::string _windowTitle = "Twurtle Engine", std::int16_t _windowWidth = 600, std::int16_t _windowHeight = 600) noexcept
        : m_window{std::move(_windowTitle), _windowWidth, _windowHeight} {
            graphics::gl_detail::set_render_context(m_window);
            graphics::enable_depth_test();
        }

        controller(controller const&) = delete;
        controller(controller&&) = delete;

        [[nodiscard]] auto const& timer() const noexcept { return m_timer; }

        void tick() noexcept {
            fetch_raw_events();
            graphics::clear_graphics();
            m_timer.tick(fetch_current_raw_time());
        }

        [[nodiscard]] auto const& inputs() const noexcept { return m_currentInputState; }
        [[nodiscard]] auto const& input_changes() const noexcept { return m_inputStateChanges; }

        template<typename _renderer_t, typename... _renderer_arg_t>
        void render(_renderer_t const& _renderer,
                    _renderer_arg_t&&... _rendererArg) noexcept(noexcept(_renderer(std::forward<_renderer_arg_t>(_rendererArg)...))) {
            _renderer(std::forward<_renderer_arg_t>(_rendererArg)...);
            m_window.swap_buffers();
        }

        [[nodiscard]] auto& window() noexcept { return m_window; }
        [[nodiscard]] auto const& window() const noexcept { return m_window; }

        [[nodiscard]] auto quit_received() const noexcept { return m_quitReceived; }

    private:
        graphics::window m_window;

        input::input_state m_currentInputState;
        input::input_state_changes m_inputStateChanges;

        bool m_quitReceived = false;

        system_timer m_timer = system_timer{fetch_current_raw_time()};

        std::chrono::milliseconds fetch_current_raw_time() const noexcept;

        void fetch_raw_events() noexcept;
    };
}    // namespace randomcat::engine
