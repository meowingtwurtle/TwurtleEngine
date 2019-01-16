#pragma once

#include <randomcat/engine/detail/tag_exception.hpp>

namespace randomcat::engine {
    namespace init_detail {
        struct init_error_tag {};
    }    // namespace init_detail

    using init_error = util_detail::tag_exception<init_detail::init_error_tag>;

    void init() noexcept(!"Throws on error");
}    // namespace randomcat::engine
