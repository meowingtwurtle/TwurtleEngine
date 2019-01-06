#pragma once

namespace randomcat::engine::graphics {

    template<typename Vertex>
    shader<Vertex>::shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) {
        detail::shader_id vertexID{GL_VERTEX_SHADER};

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

        detail::shader_id fragmentID{GL_FRAGMENT_SHADER};

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
            detail::program_id programID{};

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

        detail::global_shader_inputs_map().emplace(std::make_pair(m_programID.value(), std::move(_inputs)));
    }

    template<typename Vertex>
    std::vector<shader_input> const& shader<Vertex>::inputs() const noexcept {
        return detail::global_shader_inputs_map().at(m_programID);
    }
}    // namespace randomcat::engine::graphics
