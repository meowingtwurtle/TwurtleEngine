#pragma once

#include <GL/glew.h>

#include "randomcat/engine/low_level/graphics/gl_wrappers/opengl_raii_id.hpp"

namespace randomcat::engine::graphics::gl_detail {
    [[nodiscard]] inline auto make_vao() noexcept {
        opengl_raw_id id;
        glGenVertexArrays(1, &id);
        return opengl_raw_id{id};
    }

    inline void destroy_vao(opengl_raw_id _id) noexcept { glDeleteVertexArrays(1, &_id); }

    using unique_vao_id = unique_opengl_raii_id<make_vao, destroy_vao>;
    using shared_vao_id = shared_opengl_raii_id<make_vao, destroy_vao>;
    using raw_vao_id = unique_vao_id::raw_id;
}    // namespace randomcat::engine::graphics::gl_detail
