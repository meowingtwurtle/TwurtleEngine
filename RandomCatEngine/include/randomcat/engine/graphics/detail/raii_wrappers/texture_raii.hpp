#pragma once

#include <GL/glew.h>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp"

namespace randomcat::engine::graphics::gl_raii_detail {
    inline opengl_raw_id make_texture() noexcept {
        RC_GL_ERROR_GUARD("creating texture id");

        opengl_raw_id id;
        glGenTextures(1, &id);
        return id;
    }

    inline void destroy_texture(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying texture");

        glDeleteTextures(1, &_id);
    }

    using unique_texture_id = unique_opengl_raii_id<make_texture, destroy_texture>;
    using shared_texture_id = shared_opengl_raii_id<make_texture, destroy_texture>;
}    // namespace randomcat::engine::graphics::gl_raii_detail
