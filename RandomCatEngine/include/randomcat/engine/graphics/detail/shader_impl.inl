#pragma once

namespace randomcat::engine::graphics {
    namespace detail {
        inline detail::shader_id compile_shader(GLenum _type, char const* _source) noexcept(false) {
            auto shaderID = detail::shader_id(_type);

            glShaderSource(shaderID, 1, &_source, nullptr);
            glCompileShader(shaderID);

            int success = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr int BUFFER_LEN = 512;

                std::array<char, BUFFER_LEN> errorBuffer{};
                glGetShaderInfoLog(shaderID, BUFFER_LEN, nullptr, errorBuffer.data());

                throw std::runtime_error{std::string{"Error compiling shader: "} + errorBuffer.data()};
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
        inline detail::program_id link_program(Shaders const&... _shaders) noexcept(false) {
            static_assert((std::is_same_v<Shaders, detail::shader_id> && ...), "Arguments must all be shader_ids");

            detail::program_id programID;

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
                    throw std::runtime_error{std::string{"Error linking program: "} + errorBuffer.data()};
                }
            }

            return programID;
        }
    }    // namespace detail

    template<typename Vertex>
    shader<Vertex>::shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs)
    : shader(detail::link_program(detail::compile_vertex_shader(_vertex), detail::compile_fragment_shader(_fragment)), std::move(_inputs)) {}
}    // namespace randomcat::engine::graphics
