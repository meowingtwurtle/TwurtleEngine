#include <stdexcept>
#include <unordered_map>

#include <GL/glew.h>

#include <randomcat/engine/detail/log.hpp>
#include <randomcat/engine/graphics/detail/default_vertex.hpp>
#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/shader.hpp>

namespace randomcat::engine::graphics {
    using detail::default_vertex;

    namespace detail {
        void activate_program(gl_raii_detail::shared_program_id const& _program) noexcept {
            RC_GL_ERROR_GUARD("activating program");

            glUseProgram(_program);
        }
    }    // namespace detail

    using gl_raii_detail::shared_program_id;
    using gl_raii_detail::unique_shader_id;

    const_shader_uniform_manager::active_lock::active_lock(gl_raii_detail::shared_program_id const& _programID) noexcept
    : m_programID(_programID) {
        auto oldActiveShader = get_active_program();
        if (m_programID.value() != oldActiveShader) {
            m_oldID = oldActiveShader;
            set_active_program(m_programID.value());
        }
    }

    const_shader_uniform_manager::active_lock::~active_lock() noexcept {
        if (m_oldID.has_value()) set_active_program(*m_oldID);
    }

    GLuint const_shader_uniform_manager::active_lock::get_active_program() noexcept {
        RC_GL_ERROR_GUARD("getting active program");

        GLint value;
        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        return static_cast<uint>(value);
    }

    void const_shader_uniform_manager::active_lock::set_active_program(GLuint _id) noexcept {
        RC_GL_ERROR_GUARD("activating program");

        glUseProgram(_id);
    }

    GLint const_shader_uniform_manager::get_uniform_location(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting uniform location");

        return glGetUniformLocation(program(), _name.data());
    }

    bool const_shader_uniform_manager::get_bool(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting bool uniform");

        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return static_cast<bool>(result);
    }

    int const_shader_uniform_manager::get_int(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting int uniform");

        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    float const_shader_uniform_manager::get_float(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting float uniform");

        auto l = make_active_lock();

        GLfloat result;
        glGetnUniformfv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    glm::vec3 const_shader_uniform_manager::get_vec3(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting vec3 uniform");

        auto l = make_active_lock();

        GLfloat result[3];
        glGetnUniformfv(program(), get_uniform_location(_name), 3, result);

        return glm::vec3(result[0], result[1], result[2]);
    }

    glm::mat4 const_shader_uniform_manager::get_mat4(std::string_view _name) const noexcept {
        RC_GL_ERROR_GUARD("getting mat4 uniform");

        auto l = make_active_lock();

        GLfloat result[16];
        glGetnUniformfv(program(), get_uniform_location(_name), 16, result);

        return glm::mat4{result[0],
                         result[1],
                         result[2],
                         result[3],
                         result[4],
                         result[5],
                         result[6],
                         result[7],
                         result[8],
                         result[9],
                         result[10],
                         result[11],
                         result[12],
                         result[13],
                         result[14],
                         result[15]};
    }

    void shader_uniform_manager::set_bool(std::string_view _name, bool _value) noexcept {
        RC_GL_ERROR_GUARD("setting bool uniform");

        auto l = make_active_lock();
        glUniform1i(get_uniform_location(_name), _value);
    }

    void shader_uniform_manager::set_int(std::string_view _name, int _value) noexcept {
        RC_GL_ERROR_GUARD("setting int uniform");

        auto l = make_active_lock();
        glUniform1i(get_uniform_location(_name), _value);
    }

    void shader_uniform_manager::set_float(std::string_view _name, float _value) noexcept {
        RC_GL_ERROR_GUARD("setting float uniform");

        auto l = make_active_lock();
        glUniform1f(get_uniform_location(_name), _value);
    }

    void shader_uniform_manager::set_vec3(std::string_view _name, glm::vec3 const& _value) noexcept {
        RC_GL_ERROR_GUARD("setting vec3 uniform");

        auto l = make_active_lock();
        glUniform3fv(get_uniform_location(_name), 1, reinterpret_cast<float const*>(&_value));
    }

    void shader_uniform_manager::set_mat4(std::string_view _name, glm::mat4 const& _value) noexcept {
        RC_GL_ERROR_GUARD("setting mat4 uniform");

        auto l = make_active_lock();
        glUniformMatrix4fv(get_uniform_location(_name), 1, false, reinterpret_cast<float const*>(&_value));
    }
}    // namespace randomcat::engine::graphics
