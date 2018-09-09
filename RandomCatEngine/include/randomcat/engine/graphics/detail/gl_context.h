#pragma once

namespace randomcat::engine::graphics {
    class window;

    struct context_creation_failed {};

    namespace detail {
        void setContext(window const& _window);
    }
}    // namespace randomcat::engine::graphics
