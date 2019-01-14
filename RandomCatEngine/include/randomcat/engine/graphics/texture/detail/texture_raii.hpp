#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::texture::detail {
    using opengl_raw_id = randomcat::engine::graphics::detail::opengl_raw_id;

    inline opengl_raw_id createTexture() noexcept {
        RC_GL_ERROR_GUARD("creating texture id");

        opengl_raw_id id;
        glGenTextures(1, &id);
        return id;
    }

    inline void deleteTexture(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying texture");

        glDeleteTextures(1, &_id);
    }

    using unique_texture_id = graphics::detail::unique_opengl_raii_id<createTexture, deleteTexture>;
}    // namespace randomcat::engine::graphics::texture::detail
