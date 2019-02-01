#include "randomcat/engine/graphics/shader.hpp"

#include <stdexcept>
#include <unordered_map>

#include <GL/glew.h>

#include "randomcat/engine/detail/log.hpp"
#include "randomcat/engine/graphics/detail/default_vertex.hpp"
#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"

namespace randomcat::engine::graphics {
    namespace shader_detail {
        void activate_program(gl_raii_detail::shared_program_id const& _program) noexcept {
            RC_GL_ERROR_GUARD("activating program");

            glUseProgram(_program);
        }
    }    // namespace shader_detail

    using gl_raii_detail::shared_program_id;
    using gl_raii_detail::unique_shader_id;

    shader_detail::program_active_lock::program_active_lock(gl_raii_detail::shared_program_id const& _programID) noexcept
    : m_programID(_programID) {
        auto oldActiveShader = get_active_program();
        if (m_programID.value() != oldActiveShader) {
            m_oldID = oldActiveShader;
            set_active_program(m_programID.value());
        }
    }

    shader_detail::program_active_lock::~program_active_lock() noexcept {
        if (m_oldID.has_value()) set_active_program(*m_oldID);
    }

    GLuint shader_detail::program_active_lock::get_active_program() noexcept {
        RC_GL_ERROR_GUARD("getting active program");

        GLint value;
        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        return static_cast<uint>(value);
    }

    void shader_detail::program_active_lock::set_active_program(GLuint _id) noexcept {
        RC_GL_ERROR_GUARD("activating program");

        glUseProgram(_id);
    }
}    // namespace randomcat::engine::graphics
