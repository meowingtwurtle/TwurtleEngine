#include <stdexcept>
#include <unordered_map>

#include <GL/glew.h>

#include <randomcat/engine/detail/log.hpp>
#include <randomcat/engine/graphics/detail/default_vertex.hpp>
#include <randomcat/engine/graphics/shader.hpp>

namespace randomcat::engine::graphics {
    using detail::default_vertex;

    namespace {
        std::unordered_map<unsigned, std::vector<shader_input>> g_shaderInputsMap{};
    }

    using detail::program_id;
    using detail::shader_id;

    shader::shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) {
        shader_id vertexID{GL_VERTEX_SHADER};

        {
            glShaderSource(vertexID, 1, &_vertex, nullptr);
            glCompileShader(vertexID);

            int success = 0;
            glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr int BUFFER_LEN = 512;
                std::array<char, BUFFER_LEN> errorBuffer{};
                glGetShaderInfoLog(vertexID, BUFFER_LEN, nullptr, errorBuffer.data());

                throw std::runtime_error{std::string{"Error compiling vertex shader:"} + errorBuffer.data()};
            }
        }

        shader_id fragmentID{GL_FRAGMENT_SHADER};

        {
            glShaderSource(fragmentID, 1, &_fragment, nullptr);
            glCompileShader(fragmentID);

            int success = 0;
            glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr int BUFFER_LEN = 512;
                std::array<char, BUFFER_LEN> errorBuffer{};
                glGetShaderInfoLog(fragmentID, BUFFER_LEN, nullptr, errorBuffer.data());

                throw std::runtime_error{std::string{"Error compiling fragment shader:"} + errorBuffer.data()};
            }
        }

        {
            program_id programID{};

            {
                glAttachShader(programID, vertexID);
                glAttachShader(programID, fragmentID);
                glLinkProgram(programID);

                int success = 0;
                glGetProgramiv(programID, GL_LINK_STATUS, &success);

                if (!success) {
                    constexpr int BUFFER_LEN = 512;
                    std::array<char, BUFFER_LEN> errorBuffer{};

                    glGetProgramInfoLog(programID, BUFFER_LEN, nullptr, errorBuffer.data());
                    throw std::runtime_error{std::string{"Error linking shader:"} + errorBuffer.data()};
                }
            }

            m_programID = std::move(programID);
        }

        g_shaderInputsMap.emplace(std::make_pair(m_programID.value(), std::move(_inputs)));
    }

    void shader::make_active(detail::program_id _program) noexcept { glUseProgram(_program); }

    shader::const_uniform_manager::active_lock::active_lock(detail::program_id _programID) noexcept : m_programID(std::move(_programID)) {
        auto oldActiveShader = get_active_program();
        if (m_programID.value() != oldActiveShader) {
            m_oldID = oldActiveShader;
            set_active_program(m_programID.value());
        }
    }

    shader::const_uniform_manager::active_lock::~active_lock() noexcept {
        if (m_oldID.has_value()) set_active_program(*m_oldID);
    }

    GLuint shader::const_uniform_manager::active_lock::get_active_program() noexcept {
        GLint value;
        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        return static_cast<uint>(value);
    }

    void shader::const_uniform_manager::active_lock::set_active_program(GLuint _id) noexcept { glUseProgram(_id); }

    GLint shader::const_uniform_manager::get_uniform_location(std::string_view _name) const noexcept {
        return glGetUniformLocation(m_programID, _name.data());
    }

    bool shader::const_uniform_manager::get_bool(std::string_view _name) const noexcept {
        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(m_programID, get_uniform_location(_name), 1, &result);

        return static_cast<bool>(result);
    }

    int shader::const_uniform_manager::get_int(std::string_view _name) const noexcept {
        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(m_programID, get_uniform_location(_name), 1, &result);

        return result;
    }

    float shader::const_uniform_manager::get_float(std::string_view _name) const noexcept {
        auto l = make_active_lock();

        GLfloat result;
        glGetnUniformfv(m_programID, get_uniform_location(_name), 1, &result);

        return result;
    }

    glm::vec3 shader::const_uniform_manager::get_vec3(std::string_view _name) const noexcept {
        auto l = make_active_lock();

        GLfloat result[3];
        glGetnUniformfv(m_programID, get_uniform_location(_name), 3, result);

        return glm::vec3(result[0], result[1], result[2]);
    }

    glm::mat4 shader::const_uniform_manager::get_mat4(std::string_view _name) const noexcept {
        auto l = make_active_lock();

        GLfloat result[16];
        glGetnUniformfv(m_programID, get_uniform_location(_name), 16, result);

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

    void shader::uniform_manager::set_bool(std::string_view _name, bool _value) noexcept {
        auto l = make_active_lock();
        glUniform1i(get_uniform_location(_name), _value);
    }

    void shader::uniform_manager::set_int(std::string_view _name, int _value) noexcept {
        auto l = make_active_lock();
        glUniform1i(get_uniform_location(_name), _value);
    }

    void shader::uniform_manager::set_float(std::string_view _name, float _value) noexcept {
        auto l = make_active_lock();
        glUniform1f(get_uniform_location(_name), _value);
    }

    void shader::uniform_manager::set_vec3(std::string_view _name, glm::vec3 const& _value) noexcept {
        auto l = make_active_lock();
        glUniform3fv(get_uniform_location(_name), 1, reinterpret_cast<float const*>(&_value));
    }

    void shader::uniform_manager::set_mat4(std::string_view _name, glm::mat4 const& _value) noexcept {
        auto l = make_active_lock();
        glUniformMatrix4fv(get_uniform_location(_name), 1, false, reinterpret_cast<float const*>(&_value));
    }

    std::vector<shader_input> const& shader::inputs() const noexcept { return g_shaderInputsMap.at(m_programID); }
}    // namespace randomcat::engine::graphics
