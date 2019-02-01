#pragma once

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"

namespace randomcat::engine::graphics {
    namespace shader_detail {
        inline gl_raii_detail::unique_shader_id compile_shader(GLenum _type, char const* _source) noexcept(false) {
            RC_GL_ERROR_GUARD("compiling shader");

            auto shaderID = gl_raii_detail::unique_shader_id(_type);

            glShaderSource(shaderID, 1, &_source, nullptr);
            glCompileShader(shaderID);

            int success = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr int BUFFER_LEN = 512;

                std::array<char, BUFFER_LEN> errorBuffer{};
                glGetShaderInfoLog(shaderID, BUFFER_LEN, nullptr, errorBuffer.data());

                throw shader_init_error{std::string{"Error compiling shader: "} + errorBuffer.data()};
            }

            return shaderID;
        }

        inline decltype(auto) compile_vertex_shader(char const* _source) noexcept(noexcept(compile_shader(GL_VERTEX_SHADER, _source))) {
            return compile_shader(GL_VERTEX_SHADER, _source);
        }

        inline decltype(auto) compile_fragment_shader(char const* _source) noexcept(noexcept(compile_shader(GL_FRAGMENT_SHADER, _source))) {
            return compile_shader(GL_FRAGMENT_SHADER, _source);
        }

        template<typename... Shaders>
        inline gl_raii_detail::shared_program_id link_program(Shaders const&... _shaders) noexcept(false) {
            static_assert((std::is_same_v<Shaders, gl_raii_detail::unique_shader_id> && ...), "Arguments must all be shader_ids");

            RC_GL_ERROR_GUARD("linking program");

            gl_raii_detail::shared_program_id programID;

            {
                // Attach all shaders
                ((glAttachShader(programID, _shaders)), ...);

                glLinkProgram(programID);

                int success = 0;
                glGetProgramiv(programID, GL_LINK_STATUS, &success);

                if (!success) {
                    constexpr int BUFFER_LEN = 512;
                    std::array<char, BUFFER_LEN> errorBuffer{};

                    glGetProgramInfoLog(programID, BUFFER_LEN, nullptr, errorBuffer.data());
                    throw shader_init_error{std::string{"Error linking program: "} + errorBuffer.data()};
                }
            }

            return programID;
        }

        inline GLuint program_binary_size(gl_raii_detail::shared_program_id const& _program) {
            RC_GL_ERROR_GUARD("getting program size");

            GLint size;
            glGetProgramiv(_program, GL_PROGRAM_BINARY_LENGTH, &size);
            return GLuint(size);
        }
    }    // namespace shader_detail

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities>::shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs)
    : shader(shader_detail::link_program(shader_detail::compile_vertex_shader(_vertex), shader_detail::compile_fragment_shader(_fragment)),
             std::move(_inputs)) {}

    namespace shader_detail {
        inline gl_raii_detail::shared_program_id clone_program(gl_raii_detail::shared_program_id const& _program) noexcept {
            RC_GL_ERROR_GUARD("cloning program");

            auto const programSize = program_binary_size(_program);
            auto binary = std::vector<char>(programSize);
            GLenum binaryFormat;

            glGetProgramBinary(_program, programSize, nullptr, &binaryFormat, binary.data());

            auto newProgram = gl_raii_detail::shared_program_id();
            glProgramBinary(newProgram, binaryFormat, binary.data(), programSize);

            return newProgram;
        }
    }    // namespace shader_detail

    template<typename Vertex, typename Capabilities>
    template<typename NewVertex>
    shader<NewVertex, Capabilities> shader<Vertex, Capabilities>::reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept {
        return shader<NewVertex, Capabilities>(shader_detail::clone_program(program()), std::move(_inputs));
    }

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities> shader<Vertex, Capabilities>::reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept {
        return reinterpret_vertex_and_inputs<Vertex>(std::move(_inputs));
    }

    template<typename Vertex, typename Capabilities>
    template<typename NewVertex>
    shader<NewVertex, Capabilities> shader<Vertex, Capabilities>::reinterpret_vertex() const noexcept {
        return reinterpret_vertex_and_inputs<NewVertex>(inputs());
    }

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities> shader<Vertex, Capabilities>::clone() const noexcept {
        return reinterpret_vertex<Vertex>();
    }

    template<typename Vertex, typename Capabilities>
    template<typename NewVertex>
    shader<NewVertex, Capabilities> shader_view<Vertex, Capabilities>::reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept {
        return shader<NewVertex, Capabilities>(shader_detail::clone_program(program()), std::move(_inputs));
    }

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities> shader_view<Vertex, Capabilities>::reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept {
        return reinterpret_vertex_and_inputs<Vertex>(std::move(_inputs));
    }

    template<typename Vertex, typename Capabilities>
    template<typename NewVertex>
    shader<NewVertex, Capabilities> shader_view<Vertex, Capabilities>::reinterpret_vertex() const noexcept {
        return reinterpret_vertex_and_inputs<NewVertex>(inputs());
    }

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities> shader_view<Vertex, Capabilities>::clone() const noexcept {
        return reinterpret_vertex<Vertex>();
    }

    template<typename Capabilities>
    GLint const_shader_uniform_manager<Capabilities>::get_uniform_location(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting uniform location");

        auto loc = glGetUniformLocation(program(), _name.c_str());
        if (loc == -1) throw shader_no_such_uniform_error("No such uniform: " + _name);

        return loc;
    }

    template<typename Capabilities>
    bool const_shader_uniform_manager<Capabilities>::get_bool(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting bool uniform");

        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return static_cast<bool>(result);
    }

    template<typename Capabilities>
    int const_shader_uniform_manager<Capabilities>::get_int(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting int uniform");

        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    template<typename Capabilities>
    float const_shader_uniform_manager<Capabilities>::get_float(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting float uniform");

        auto l = make_active_lock();

        GLfloat result;
        glGetnUniformfv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    template<typename Capabilities>
    glm::vec3 const_shader_uniform_manager<Capabilities>::get_vec3(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting vec3 uniform");

        auto l = make_active_lock();

        GLfloat result[3];
        glGetnUniformfv(program(), get_uniform_location(_name), 3, result);

        return glm::vec3(result[0], result[1], result[2]);
    }

    template<typename Capabilities>
    glm::mat4 const_shader_uniform_manager<Capabilities>::get_mat4(std::string const& _name) const {
        RC_GL_ERROR_GUARD("getting mat4 uniform");

        auto l = make_active_lock();

        GLfloat result[16];
        glGetnUniformfv(program(), get_uniform_location(_name), 16, result);

        // clang-format off

        return glm::mat4{
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
        };

        // clang-format on
    }

    template<typename Capabilities>
    void shader_uniform_manager<Capabilities>::set_bool(std::string const& _name, bool _value) {
        RC_GL_ERROR_GUARD("setting bool uniform");

        auto l = this->make_active_lock();
        glUniform1i(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_manager<Capabilities>::set_int(std::string const& _name, int _value) {
        RC_GL_ERROR_GUARD("setting int uniform");

        auto l = this->make_active_lock();
        glUniform1i(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_manager<Capabilities>::set_float(std::string const& _name, float _value) {
        RC_GL_ERROR_GUARD("setting float uniform");

        auto l = this->make_active_lock();
        glUniform1f(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_manager<Capabilities>::set_vec3(std::string const& _name, glm::vec3 const& _value) {
        RC_GL_ERROR_GUARD("setting vec3 uniform");

        auto l = this->make_active_lock();
        glUniform3fv(this->get_uniform_location(_name), 1, reinterpret_cast<float const*>(&_value));
    }

    template<typename Capabilities>
    void shader_uniform_manager<Capabilities>::set_mat4(std::string const& _name, glm::mat4 const& _value) {
        RC_GL_ERROR_GUARD("setting mat4 uniform");

        auto l = this->make_active_lock();
        glUniformMatrix4fv(this->get_uniform_location(_name), 1, false, reinterpret_cast<float const*>(&_value));
    }
}    // namespace randomcat::engine::graphics
