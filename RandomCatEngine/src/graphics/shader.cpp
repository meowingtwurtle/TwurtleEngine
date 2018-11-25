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

        m_programID = programID;

        g_shaderInputsMap.emplace(std::make_pair(m_programID.value(), std::move(_inputs)));
    }

    void shader::make_active() const noexcept { glUseProgram(m_programID); }

    void shader::uniform_manager::set_bool(std::string const& _name, bool _value) noexcept {
        make_active();
        glUniform1i(glGetUniformLocation(m_programID, _name.c_str()), _value);
    }

    void shader::uniform_manager::set_int(std::string const& _name, int _value) noexcept {
        make_active();
        glUniform1i(glGetUniformLocation(m_programID, _name.c_str()), _value);
    }

    void shader::uniform_manager::set_float(std::string const& _name, float _value) noexcept {
        make_active();
        glUniform1f(glGetUniformLocation(m_programID, _name.c_str()), _value);
    }

    void shader::uniform_manager::set_vec3(std::string const& _name, glm::vec3 const& _value) noexcept {
        make_active();
        glUniform3fv(glGetUniformLocation(m_programID, _name.c_str()), 1, reinterpret_cast<float const*>(&_value));
    }

    void shader::uniform_manager::set_mat4(std::string const& _name, glm::mat4 const& _value) noexcept {
        make_active();
        glUniformMatrix4fv(glGetUniformLocation(m_programID, _name.c_str()), 1, false, reinterpret_cast<float const*>(&_value));
    }

    void shader::uniform_manager::make_active() const noexcept { glUseProgram(m_programID); }

    std::vector<shader_input> const& shader::inputs() const noexcept { return g_shaderInputsMap.at(m_programID); }
}    // namespace randomcat::engine::graphics
