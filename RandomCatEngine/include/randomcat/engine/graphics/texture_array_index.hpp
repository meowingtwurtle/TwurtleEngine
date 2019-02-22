#pragma once

#include "randomcat/engine/detail/safe_int.hpp"
#include "randomcat/engine/graphics/detail/gl_types.hpp"

namespace randomcat::engine::graphics {
    namespace texture_index_detail {
        struct texture_index_tag {};
    }    // namespace texture_index_detail
    using texture_array_index = util_detail::safe_integer<GLint, texture_index_detail::texture_index_tag>;

    static_assert(sizeof(texture_array_index) == sizeof(GLint));
    static_assert(offsetof(texture_array_index, value) == 0);
}    // namespace randomcat::engine::graphics
