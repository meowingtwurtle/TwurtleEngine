#include <randomcat/engine/graphics/camera.h>
#include <randomcat/engine/graphics/detail/default_vertex.h>
#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
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

    using detail::default_vertex;

    shader camera::camera_shader() {
        shader ourShader(DEFAULT_VERTEX_SHADER,
                         DEFAULT_FRAGMENT_SHADER,
                         {{0, 3, GL_FLOAT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, location))},
                          {1, 2, GL_FLOAT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, texCoord))},
                          {2, 1, GL_INT, false, sizeof(default_vertex), reinterpret_cast<void*>(offsetof(default_vertex, layerNum))}});

        shader::uniform_manager shaderUniforms = ourShader.uniforms();
        shaderUniforms.set_mat4("model", glm::mat4{1.0f});
        shaderUniforms.set_mat4("view", glm::mat4{1.0f});
        shaderUniforms.set_mat4("projection", glm::mat4{1.0f});

        return ourShader;
    }
}    // namespace randomcat::engine::graphics