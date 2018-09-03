#pragma once

#include <memory>

#include <twurtle/detail/default_renderer.h>
#include <twurtle/init.h>
#include <twurtle/renderer.h>
#include <twurtle/shader.h>
#include <twurtle/window.h>

namespace randomcat::graphics {
    class engine {
    public:
        engine(std::string _windowTitle = "Twurtle Engine", int _windowWidth = 600, int _windowHeight = 600)
        : m_window{std::move(_windowTitle), _windowWidth, _windowHeight} {
            setContext(m_window);
        }

        engine(engine const&) = delete;
        engine(engine&&) = delete;

        void beforeUpdate() {}

        void render(renderer const& _renderer) {
            _renderer.render();
            m_window.swapBuffers();
        }

    private:
        window m_window;
    };
}    // namespace randomcat::graphics
