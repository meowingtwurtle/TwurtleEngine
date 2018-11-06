#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.h>

namespace randomcat::engine::graphics::texture::detail {
    using opengl_raw_id = randomcat::engine::graphics::detail::opengl_raw_id;

    inline opengl_raw_id createTexture() noexcept {
        opengl_raw_id id;
        glGenTextures(1, &id);
        return id;
    }

    inline void deleteTexture(opengl_raw_id _id) noexcept { glDeleteTextures(1, &_id); }

    using texture_id = randomcat::engine::graphics::detail::opengl_raii_id<createTexture, deleteTexture>;
}    // namespace randomcat::engine::graphics::texture::detail
