#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    inline opengl_raw_id make_shader(GLenum _type) noexcept {
        RC_GL_ERROR_GUARD("creating shader id");

        return opengl_raw_id{glCreateShader(_type)};
    }

    inline void destroy_shader(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying shader");

        glDeleteShader(_id);
    }

    inline opengl_raw_id make_program() noexcept {
        RC_GL_ERROR_GUARD("creating program id");

        return opengl_raw_id{glCreateProgram()};
    }

    inline void destroy_program(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying program");

        glDeleteProgram(_id);
    }

    using shader_id = opengl_raii_id<make_shader, destroy_shader, GLenum>;
    using program_id = opengl_raii_id<make_program, destroy_program>;
}    // namespace randomcat::engine::graphics::detail
