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
}    // namespace randomcat::engine::graphics
