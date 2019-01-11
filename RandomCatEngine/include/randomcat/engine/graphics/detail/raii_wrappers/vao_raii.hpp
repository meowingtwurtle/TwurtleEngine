#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    inline decltype(auto) makeVAO() noexcept {
        RC_GL_ERROR_GUARD("creating vao id");

        unsigned id;
        glGenVertexArrays(1, &id);
        return opengl_raw_id{id};
    }

    inline void destroyVAO(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying vao");

        glDeleteVertexArrays(1, &_id);
    }

    using vao_id = opengl_raii_id<makeVAO, destroyVAO>;
}    // namespace randomcat::engine::graphics::detail
