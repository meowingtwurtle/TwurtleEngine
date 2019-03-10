#include "randomcat/engine/graphics/camera.hpp"

#include "randomcat/engine/graphics/detail/default_vertex.hpp"
#include "randomcat/engine/graphics/shader.hpp"

namespace randomcat::engine::graphics {
    namespace {
        constexpr const char* const DEFAULT_VERTEX_SHADER = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in int aLayerNum;

            out vec2 texCoord;
            flat out int layerNum;

            uniform mat4 camera;
            
            void main()
            {
                gl_Position = camera * vec4(aPos, 1.0);
                texCoord = aTexCoord;
                layerNum = aLayerNum;
            }
        )";
        constexpr const char* const DEFAULT_FRAGMENT_SHADER = R"(
            #version 330 core
            out vec4 FragColor;
              
            in vec2 texCoord;
            flat in int layerNum;

            uniform sampler2DArray textures;
            
            void main()
            {
                vec4 color = texture(textures, vec3(texCoord, layerNum));
                if (color.a == 0.0) discard;
                FragColor = color;
            }
        )";
    }    // namespace

    shader<default_vertex, shader_capabilities<camera>> camera::camera_shader() {
        shader<default_vertex, shader_capabilities<camera>> ourShader(DEFAULT_VERTEX_SHADER,
                                                                      DEFAULT_FRAGMENT_SHADER,
                                                                      {{0,
                                                                        shader_input::vec3_type,
                                                                        shader_input_storage_type::floating_point,
                                                                        offsetof(default_vertex, loc) + offsetof(default_vertex::location, value),
                                                                        sizeof(default_vertex)},
                                                                       {1,
                                                                        shader_input::vec2_type,
                                                                        shader_input_storage_type::floating_point,
                                                                        offsetof(default_vertex, tex) + offsetof(default_vertex::texture, coord),
                                                                        sizeof(default_vertex)},
                                                                       {2,
                                                                        shader_input::int_type,
                                                                        shader_input_storage_type::signed_int,
                                                                        offsetof(default_vertex, tex) + offsetof(default_vertex::texture, layer),
                                                                        sizeof(default_vertex)}});

        ourShader.uniforms().set_mat4("camera", glm::mat4{1.0f});

        return ourShader;
    }
}    // namespace randomcat::engine::graphics
