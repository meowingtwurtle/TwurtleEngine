#pragma once

#include <GL/glew.h>

#include "randomcat/engine/graphics/detail/raii_wrappers/buffer_raii.hpp"

namespace randomcat::engine::graphics::gl_detail {
    struct vbo_tag {};

    using shared_vbo_id = shared_buffer_id<vbo_tag>;
    using unique_vbo_id = unique_buffer_id<vbo_tag>;
}    // namespace randomcat::engine::graphics::gl_detail
