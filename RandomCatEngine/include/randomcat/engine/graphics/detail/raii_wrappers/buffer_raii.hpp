#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    inline decltype(auto) raw_make_buffer() noexcept {
        RC_GL_ERROR_GUARD("creating buffer id");

        unsigned id;
        glGenBuffers(1, &id);
        return opengl_raw_id{id};
    }

    inline void raw_destroy_buffer(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying buffer");

        glDeleteBuffers(1, &_id);
    }

    template<typename Tag>
    inline decltype(auto) make_buffer() noexcept(noexcept(raw_make_buffer())) {
        return raw_make_buffer();
    }

    template<typename Tag>
    inline decltype(auto) destroy_buffer(opengl_raw_id _id) noexcept(noexcept(raw_destroy_buffer(_id))) {
        return raw_destroy_buffer(_id);
    }

    template<typename Tag>
    using unique_buffer_id = unique_opengl_raii_id<make_buffer<Tag>, destroy_buffer<Tag>>;
}    // namespace randomcat::engine::graphics::detail
