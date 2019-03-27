#include "randomcat/engine/graphics/camera.hpp"

#include "randomcat/engine/graphics/detail/default_vertex.hpp"
#include "randomcat/engine/graphics/lights.hpp"
#include "randomcat/engine/graphics/shader.hpp"

namespace randomcat::engine::graphics {
    namespace {
        constexpr const char* const DEFAULT_VERTEX_SHADER = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in int aLayerNum;
            layout (location = 3) in vec3 aNormal;

            out vec2 texCoord;
            out vec3 normal;
            flat out int layerNum;
            out vec3 fragPos;

            uniform mat4 camera;
            
            void main()
            {
                gl_Position = camera * vec4(aPos, 1.0);
                texCoord = aTexCoord;
                layerNum = aLayerNum;
                normal = aNormal;
                fragPos = aPos;
            }
        )";
        constexpr const char* const DEFAULT_FRAGMENT_SHADER = R"(
            #version 330 core
            out vec4 FragColor;
              
            in vec2 texCoord;
            in vec3 normal;
            flat in int layerNum;
            in vec3 fragPos;
            uniform vec3 viewPos;

            uniform sampler2DArray textures;

            struct Material {
                vec3 ambient;
                vec3 diffuse;
                vec3 specular;
                float shininess;
            }; 

            uniform Material material;

            struct Light {
                vec3 position;
              
                vec3 ambient;
                vec3 diffuse;
                vec3 specular;

                float constant;
                float linear;
                float quadratic;
            };
            
#define MAX_LIGHTS 128

            uniform Light lights[MAX_LIGHTS];
            uniform int lightsUsed;
            
            void main()
            {
                vec4 color = texture(textures, vec3(texCoord, layerNum));
                if (color.a == 0.0) discard;

                vec3 totalLight = vec3(0, 0, 0);

                for (int i = 0; i < lightsUsed; ++i) {
                    Light light = lights[i];

                    vec3 ambient = light.ambient * material.ambient; 

                    vec3 norm = normalize(normal);       
                    vec3 lightDir = normalize(light.position - fragPos); 
                    float diff = max(dot(norm, lightDir), 0.0);
                    vec3 diffuse = light.diffuse * (diff * material.diffuse);

                    vec3 viewDir = normalize(viewPos - fragPos);
                    vec3 reflectDir = reflect(-lightDir, norm);
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                    vec3 specular = light.specular * (spec * material.specular);

                    float distance = length(light.position - fragPos);
                    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

                    totalLight += (ambient + diffuse + specular) * attenuation;
                }

                FragColor = vec4(min(totalLight, 1.0), 1.0) * color;
            }
        )";
    }    // namespace

    shader<default_vertex, shader_capabilities<camera, light_handler>> camera::camera_shader() {
        shader<default_vertex, shader_capabilities<camera, light_handler>>
            ourShader(DEFAULT_VERTEX_SHADER,
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
                        sizeof(default_vertex)},
                       {3, shader_input::vec3_type, shader_input_storage_type::floating_point, offsetof(default_vertex, normal), sizeof(default_vertex)}});

        auto uniforms = ourShader.uniforms();

        uniforms.set_mat4("camera", glm::mat4{1.0f});

        uniforms.set_vec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        uniforms.set_vec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        uniforms.set_vec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        uniforms.set_float("material.shininess", 32.0f);

        return ourShader;
    }
}    // namespace randomcat::engine::graphics
