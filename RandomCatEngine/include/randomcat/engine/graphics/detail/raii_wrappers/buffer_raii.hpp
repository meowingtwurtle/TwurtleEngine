#pragma once

#include <GL/glew.h>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp"

namespace randomcat::engine::graphics::gl_detail {
    [[nodiscard]] inline auto raw_make_buffer() noexcept {
        RC_GL_ERROR_GUARD("creating buffer id");

        opengl_raw_id id;
        glGenBuffers(1, &id);
        return id;
    }

    inline void raw_destroy_buffer(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying buffer");

        glDeleteBuffers(1, &_id);
    }

    template<typename Tag>
    [[nodiscard]] inline decltype(auto) make_buffer() noexcept(noexcept(raw_make_buffer())) {
        return raw_make_buffer();
    }

    template<typename Tag>
    inline decltype(auto) destroy_buffer(opengl_raw_id _id) noexcept(noexcept(raw_destroy_buffer(_id))) {
        return raw_destroy_buffer(_id);
    }

    template<typename Tag>
    using unique_buffer_id = unique_opengl_raii_id<make_buffer<Tag>, destroy_buffer<Tag>>;

    template<typename Tag>
    using shared_buffer_id = shared_opengl_raii_id<make_buffer<Tag>, destroy_buffer<Tag>>;

    template<typename Tag>
    using raw_buffer_id = typename unique_buffer_id<Tag>::raw_id;
}    // namespace randomcat::engine::graphics::gl_detail
