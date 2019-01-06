#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/buffer_raii.hpp>

namespace randomcat::engine::graphics::detail {
    struct vbo_tag {};

    using vbo_id = buffer_id<vbo_tag>;
}    // namespace randomcat::engine::graphics::detail
