#pragma once

namespace randomcat::engine::graphics {
    class window;

    namespace detail {
        void set_render_context(window const& _window) noexcept(!"Throws on error");
    }
}    // namespace randomcat::engine::graphics
