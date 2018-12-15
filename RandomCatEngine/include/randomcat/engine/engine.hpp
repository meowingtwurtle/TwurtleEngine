#pragma once

#include <chrono>
#include <memory>

#include <randomcat/engine/graphics/window.hpp>
#include <randomcat/engine/init.hpp>

namespace randomcat::engine {
    class engine {
    public:
        explicit engine(std::string _windowTitle = "Twurtle Engine", int _windowWidth = 600, int _windowHeight = 600) noexcept(false)
        : m_window{std::move(_windowTitle), _windowWidth, _windowHeight}, startTime{fetch_current_raw_time()}, lastTickTime{startTime}, currentTickTime{startTime} {
            graphics::detail::set_render_context(m_window);
        }

        engine(engine const&) = delete;
        engine(engine&&) = delete;

        void tick() noexcept {
            lastTickTime = std::move(currentTickTime);
            currentTickTime = fetch_current_raw_time();
        }

        std::chrono::milliseconds start_time() const noexcept { return startTime; }

        std::chrono::milliseconds previous_time() const noexcept { return lastTickTime; }

        std::chrono::milliseconds current_time() const noexcept { return currentTickTime; }

        std::chrono::milliseconds delta_time() const noexcept { return current_time() - previous_time(); }

        template<typename _renderer_t, typename... _renderer_arg_t>
        void render(_renderer_t const& _renderer,
                    _renderer_arg_t&&... _rendererArg) noexcept(noexcept(_renderer(std::forward<_renderer_arg_t>(_rendererArg)...))) {
            _renderer(std::forward<_renderer_arg_t>(_rendererArg)...);
            m_window.swap_buffers();
        }

        graphics::window& window() noexcept { return m_window; }
        graphics::window const& window() const noexcept { return m_window; }

    private:
        std::chrono::milliseconds startTime;
        std::chrono::milliseconds lastTickTime;
        std::chrono::milliseconds currentTickTime;

        std::chrono::milliseconds fetch_current_raw_time() const noexcept { return std::chrono::milliseconds{SDL_GetTicks()}; }

        graphics::window m_window;
    };
}    // namespace randomcat::engine
