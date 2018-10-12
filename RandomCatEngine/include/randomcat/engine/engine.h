#pragma once

#include <memory>

#include <randomcat/engine/graphics/window.h>
#include <randomcat/engine/init.h>

namespace randomcat::engine {
    class engine {
    public:
        engine(std::string _windowTitle = "Twurtle Engine", int _windowWidth = 600, int _windowHeight = 600)
        : m_window{std::move(_windowTitle), _windowWidth, _windowHeight} {
            graphics::detail::set_render_context(m_window);
        }

        engine(engine const&) = delete;
        engine(engine&&) = delete;

        void before_update() {}

        template<typename _renderer_t, typename... _renderer_arg_t>
        void render(_renderer_t const& _renderer, _renderer_arg_t&&... _rendererArg) {
            _renderer(std::forward<_renderer_arg_t>(_rendererArg)...);
            m_window.swap_buffers();
        }

        graphics::window& window() { return m_window; }
        graphics::window const& window() const { return m_window; }

    private:
        graphics::window m_window;
    };
}    // namespace randomcat::engine
