#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    template<typename Tag>
    inline decltype(auto) makeBuffer() noexcept {
        RC_GL_ERROR_GUARD("creating buffer id");

        unsigned id;
        glGenBuffers(1, &id);
        return opengl_raw_id{id};
    }

    template<typename Tag>
    inline void destroyBuffer(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying buffer");

        glDeleteBuffers(1, &_id);
    }

    template<typename Tag>
    using buffer_id = opengl_raii_id<makeBuffer<Tag>, destroyBuffer<Tag>>;
}    // namespace randomcat::engine::graphics::detail
