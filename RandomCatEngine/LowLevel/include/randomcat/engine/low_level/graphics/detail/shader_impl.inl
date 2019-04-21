#pragma once

#include <gsl/gsl>

namespace randomcat::engine::graphics {
    namespace shader_detail {
        inline auto compile_shader(GLenum _type, std::string_view _source) noexcept(!"Throws on error") {
            auto shaderID = gl_detail::unique_shader_id(_type);

            // Third argument: array of char const*, fourth argument: array of sizes of
            // strings These arguments are given std::arrays of size 1 of the value then
            // have the data pointer extracted
            glShaderSource(shaderID.value(), 1, std::array{_source.data()}.data(), std::array{gsl::narrow<GLint>(_source.size())}.data());

            glCompileShader(shaderID.value());

            GLint success = 0;
            glGetShaderiv(shaderID.value(), GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr int BUFFER_LEN = 512;

                std::array<char, BUFFER_LEN> errorBuffer{};
                glGetShaderInfoLog(shaderID.value(), BUFFER_LEN, nullptr, errorBuffer.data());

                throw shader_init_error{std::string{"Error compiling shader: "} + errorBuffer.data()};
            }

            return shaderID;
        }

        inline decltype(auto) compile_vertex_shader(std::string_view _source) noexcept(noexcept(compile_shader(GL_VERTEX_SHADER, _source))) {
            return compile_shader(GL_VERTEX_SHADER, _source);
        }

        inline decltype(auto) compile_fragment_shader(std::string_view _source) noexcept(noexcept(compile_shader(GL_FRAGMENT_SHADER, _source))) {
            return compile_shader(GL_FRAGMENT_SHADER, _source);
        }

        template<typename... Shaders>
        inline auto link_program(Shaders const&... _shaders) noexcept(false) {
            static_assert((std::is_same_v<Shaders, gl_detail::unique_shader_id> && ...), "Arguments must all be shader_ids");

            gl_detail::unique_program_id programID;

            {
                // Attach all shaders
                ((glAttachShader(programID.value(), _shaders.value())), ...);

                glLinkProgram(programID.value());

                GLint success = 0;
                glGetProgramiv(programID.value(), GL_LINK_STATUS, &success);

                if (!success) {
                    // Raw use of int okay - constant expression
                    constexpr int BUFFER_LEN = 512;
                    std::array<char, BUFFER_LEN> errorBuffer{};

                    glGetProgramInfoLog(programID.value(), BUFFER_LEN, nullptr, errorBuffer.data());
                    throw shader_init_error{std::string{"Error linking program: "} + errorBuffer.data()};
                }
            }

            return programID;
        }

        inline auto program_binary_size(gl_detail::shared_program_id const& _program) noexcept {
            GLint size;
            glGetProgramiv(_program.value(), GL_PROGRAM_BINARY_LENGTH, &size);
            return gsl::narrow<GLuint>(size);
        }
    }    // namespace shader_detail

    template<typename Vertex, typename Capabilities>
    shader<Vertex, Capabilities>::shader(std::string_view _vertex, std::string_view _fragment, std::vector<shader_input> _inputs) noexcept(false)
    : shader(shader_detail::link_program(shader_detail::compile_vertex_shader(_vertex), shader_detail::compile_fragment_shader(_fragment)),
             std::move(_inputs)) {}

    namespace shader_detail {
        inline gl_detail::shared_program_id clone_program(gl_detail::shared_program_id const& _program) noexcept {
            auto const programSize = program_binary_size(_program);
            auto binary = std::vector<char>(programSize);
            GLenum binaryFormat;

            glGetProgramBinary(_program.value(), programSize, nullptr, &binaryFormat, binary.data());

            auto newProgram = gl_detail::shared_program_id();
            glProgramBinary(newProgram.value(), binaryFormat, binary.data(), programSize);

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
    GLint shader_uniform_reader<Capabilities>::get_uniform_location(std::string const& _name) const {
        auto loc = glGetUniformLocation(program().value(), _name.c_str());
        if (loc == -1) throw no_such_uniform_error("No such uniform: " + _name);

        return loc;
    }

    template<typename Capabilities>
    bool shader_uniform_reader<Capabilities>::get_bool(std::string const& _name) const {
        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return static_cast<bool>(result);
    }

    template<typename Capabilities>
    GLint shader_uniform_reader<Capabilities>::get_int(std::string const& _name) const {
        auto l = make_active_lock();

        GLint result;
        glGetnUniformiv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    template<typename Capabilities>
    GLfloat shader_uniform_reader<Capabilities>::get_float(std::string const& _name) const {
        auto l = make_active_lock();

        GLfloat result;
        glGetnUniformfv(program(), get_uniform_location(_name), 1, &result);

        return result;
    }

    template<typename Capabilities>
    glm::vec3 shader_uniform_reader<Capabilities>::get_vec3(std::string const& _name) const {
        auto l = make_active_lock();

        GLfloat result[3];
        glGetnUniformfv(program(), get_uniform_location(_name), 3, result);

        return glm::vec3(result[0], result[1], result[2]);
    }

    template<typename Capabilities>
    glm::mat4 shader_uniform_reader<Capabilities>::get_mat4(std::string const& _name) const {
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
    void shader_uniform_writer<Capabilities>::set_bool(std::string const& _name, bool _value) const {
        auto l = this->make_active_lock();
        glUniform1i(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_writer<Capabilities>::set_int(std::string const& _name, GLint _value) const {
        auto l = this->make_active_lock();
        glUniform1i(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_writer<Capabilities>::set_float(std::string const& _name, GLfloat _value) const {
        auto l = this->make_active_lock();
        glUniform1f(this->get_uniform_location(_name), _value);
    }

    template<typename Capabilities>
    void shader_uniform_writer<Capabilities>::set_vec3(std::string const& _name, glm::vec3 const& _value) const {
        auto l = this->make_active_lock();
        glUniform3fv(this->get_uniform_location(_name), 1, reinterpret_cast<GLfloat const*>(&_value));
    }

    template<typename Capabilities>
    void shader_uniform_writer<Capabilities>::set_mat4(std::string const& _name, glm::mat4 const& _value) const {
        auto l = this->make_active_lock();
        glUniformMatrix4fv(this->get_uniform_location(_name), 1, false, reinterpret_cast<GLfloat const*>(&_value));
    }
}    // namespace randomcat::engine::graphics
