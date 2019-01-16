#pragma once

#include <string>

#include <randomcat/engine/detail/tag_exception.hpp>

namespace randomcat::engine::graphics {
    class window;

    namespace gl_detail {
        struct render_context_init_error_tag {};
        using render_context_init_error = util_detail::tag_exception<render_context_init_error_tag>;

        void set_render_context(window const& _window) noexcept(!"Throws on error");
    }    // namespace gl_detail
}    // namespace randomcat::engine::graphics
