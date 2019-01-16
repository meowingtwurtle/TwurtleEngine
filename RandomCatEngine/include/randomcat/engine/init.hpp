#pragma once

#include <randomcat/engine/detail/tag_exception.hpp>

namespace randomcat::engine {
    namespace detail {
        struct init_error_tag {};
    }    // namespace detail

    using init_error = detail::tag_exception<detail::init_error_tag>;

    void init() noexcept(!"Throws on error");
}    // namespace randomcat::engine
