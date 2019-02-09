#pragma once

#include "randomcat/engine/detail/safe_int.hpp"

namespace randomcat::engine::graphics {
    namespace texture_index_detail {
        struct texture_index_tag {};
    }    // namespace texture_index_detail
    using texture_array_index = util_detail::safe_integer<texture_index_detail::texture_index_tag>;

    static_assert(sizeof(texture_array_index) == 4);
    static_assert(offsetof(texture_array_index, value) == 0);
}    // namespace randomcat::engine::graphics
