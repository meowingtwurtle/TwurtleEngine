#pragma once

#include <memory>

#include <randomcat/engine/graphics/renderer.h>
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

        void render(graphics::renderer const& _renderer) {
            _renderer.render();
            m_window.swap_buffers();
        }

        graphics::window& window() { return m_window; }
        graphics::window const& window() const { return m_window; }

    private:
        graphics::window m_window;
    };
}    // namespace randomcat::engine
