#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    inline opengl_raw_id makeShader(GLenum _type) noexcept {
        RC_GL_ERROR_GUARD("creating shader id");

        return opengl_raw_id{glCreateShader(_type)};
    }

    inline void destroyShader(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying shader");

        glDeleteShader(_id);
    }

    inline opengl_raw_id makeProgram() noexcept {
        RC_GL_ERROR_GUARD("creating program id");

        return opengl_raw_id{glCreateProgram()};
    }

    inline void destroyProgram(opengl_raw_id _id) noexcept {
        RC_GL_ERROR_GUARD("destroying program");

        glDeleteProgram(_id);
    }

    using shader_id = opengl_raii_id<makeShader, destroyShader, GLenum>;
    using program_id = opengl_raii_id<makeProgram, destroyProgram>;
}    // namespace randomcat::engine::graphics::detail
