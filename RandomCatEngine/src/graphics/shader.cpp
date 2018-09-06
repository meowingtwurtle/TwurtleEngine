#include <GL/glew.h>

#include <randomcat/engine/detail/log.h>
#include <randomcat/engine/graphics/detail/default_vertex.h>
#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    using detail::default_vertex;

    struct shader_compile_fail {};
    struct shader_link_fail {};

    namespace {
        static constexpr const char* const DEFAULT_VERTEX_SHADER = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in int aLayerNum;

            out vec2 texCoord;
            flat out int layerNum;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
                texCoord = aTexCoord;
                layerNum = aLayerNum;
            }
        )";
        static constexpr const char* const DEFAULT_FRAGMENT_SHADER = R"(
            #version 330 core
            out vec4 FragColor;
              
            in vec2 texCoord;
            flat in int layerNum;

            uniform sampler2DArray textures;
            
            void main()
            {
                FragColor = texture(textures, vec3(texCoord, layerNum));
            }
        )";
    }    // namespace

    using detail::program_id;
    using detail::shader_id;

    shader::shader()
    : shader(DEFAULT_VERTEX_SHADER,
             DEFAULT_FRAGMENT_SHADER,
             {{0, 3, GL_FLOAT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, location))},
              {1, 2, GL_FLOAT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, texCoord))},
              {2, 1, GL_INT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, layerNum))}}) {
        makeActive();

        setMat4("model", glm::mat4{1.0f});
        setMat4("view", glm::mat4{1.0f});
        setMat4("projection", glm::mat4{1.0f});
    }

    shader::shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) : m_inputs(std::move(_inputs)) {
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

                log::error(errorBuffer.data());
                throw shader_compile_fail{};
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

                log::error(errorBuffer.data());
                throw shader_compile_fail{};
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
                log::error(errorBuffer.data());
                throw shader_link_fail{};
            }
        }

        m_programID = programID;
    }

    void shader::makeActive() const { glUseProgram(m_programID); }

    void shader::setBool(std::string const& _name, bool _value) { glUniform1i(glGetUniformLocation(m_programID, _name.c_str()), _value); }

    void shader::setInt(std::string const& _name, int _value) { glUniform1i(glGetUniformLocation(m_programID, _name.c_str()), _value); }

    void shader::setFloat(std::string const& _name, float _value) { glUniform1f(glGetUniformLocation(m_programID, _name.c_str()), _value); }

    void shader::setVec3(std::string const& _name, glm::vec3 const& _value) {
        glUniform3fv(glGetUniformLocation(m_programID, _name.c_str()), 1, reinterpret_cast<float const*>(&_value));
    }

    void shader::setMat4(std::string const& _name, glm::mat4 const& _value) {
        glUniformMatrix4fv(glGetUniformLocation(m_programID, _name.c_str()), 1, false, reinterpret_cast<float const*>(&_value));
    }
}    // namespace randomcat::engine::graphics
