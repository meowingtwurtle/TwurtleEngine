#include "randomcat/engine/low_level/graphics/gl_wrappers/active_locks.hpp"

#include <GL/glew.h>
#include <gsl/gsl_util>

namespace randomcat::engine::graphics::gl_detail {
    void activate_vao(raw_vao_id _vao) noexcept { glBindVertexArray(_vao.value); }

    raw_vao_id current_vao() noexcept {
        GLint ret;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &ret);
        return raw_vao_id{gsl::narrow<opengl_raw_id>(ret)};
    }

    void activate_vbo(raw_vbo_id _vbo) noexcept { glBindBuffer(GL_ARRAY_BUFFER, _vbo.value); }

    raw_vbo_id current_vbo() noexcept {
        GLint ret;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &ret);
        return raw_vbo_id{gsl::narrow<opengl_raw_id>(ret)};
    }

    void activate_program(raw_program_id _program) noexcept { glUseProgram(_program.value); }

    raw_program_id current_program() noexcept {
        GLint ret;
        glGetIntegerv(GL_CURRENT_PROGRAM, &ret);
        return raw_program_id{gsl::narrow<opengl_raw_id>(ret)};
    }
}    // namespace randomcat::engine::graphics::gl_detail
