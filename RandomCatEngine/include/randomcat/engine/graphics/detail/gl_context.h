#pragma once

namespace randomcat::engine::graphics {
    class window;

    namespace detail {
        void set_render_context(window const& _window) noexcept(false);
    }
}    // namespace randomcat::engine::graphics
