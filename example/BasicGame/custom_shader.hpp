#include "randomcat/engine/low_level/graphics/shader.hpp"
#include "randomcat/engine/render_objects/graphics/default_vertex.hpp"
#include "randomcat/engine/utilities/graphics/camera.hpp"
#include "randomcat/engine/utilities/graphics/lights.hpp"

namespace basic_game {
    using namespace randomcat::engine::graphics;

    struct lighting_vertex {
        using location_t = default_vertex::location_t;
        location_t location;

        using texture_t = default_vertex::texture_t;
        texture_t texture;

        glm::vec3 normal;

        struct material_t {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            GLfloat shininess;
        } material;
    };

    namespace {
        constexpr const char* const DEFAULT_VERTEX_SHADER = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in int aLayerNum;
            layout (location = 3) in vec3 aNormal;
            layout (location = 4) in vec3 aMaterialAmbient;
            layout (location = 5) in vec3 aMaterialDiffuse;
            layout (location = 6) in vec3 aMaterialSpecular;
            layout (location = 7) in float aMaterialShininess;

            out vec2 texCoord;
            out vec3 normal;
            flat out int layerNum;
            out vec3 fragPos;
            out vec3 materialAmbient;
            out vec3 materialDiffuse;
            out vec3 materialSpecular;
            out float materialShininess;

            uniform mat4 camera;
            
            void main()
            {
                gl_Position = camera * vec4(aPos, 1.0);
                texCoord = aTexCoord;
                layerNum = aLayerNum;
                normal = aNormal;
                fragPos = aPos;
                materialAmbient = aMaterialAmbient;
                materialDiffuse = aMaterialDiffuse;
                materialSpecular = aMaterialSpecular;
                materialShininess = aMaterialShininess;
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

            in vec3 materialAmbient;
            in vec3 materialDiffuse;
            in vec3 materialSpecular;
            in float materialShininess;

            uniform sampler2DArray textures;

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

                    vec3 ambient = light.ambient * materialAmbient; 

                    vec3 norm = normalize(normal);       
                    vec3 lightDir = normalize(light.position - fragPos); 
                    float diff = max(dot(norm, lightDir), 0.0);
                    vec3 diffuse = light.diffuse * (diff * materialDiffuse);

                    vec3 viewDir = normalize(viewPos - fragPos);
                    vec3 reflectDir = reflect(-lightDir, norm);
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
                    vec3 specular = light.specular * (spec * materialSpecular);

                    float distance = length(light.position - fragPos);
                    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

                    totalLight += (ambient + diffuse + specular) * attenuation;
                }

                FragColor = vec4(min(totalLight, 1.0), 1.0) * color;
            }
        )";
    }    // namespace

    shader<lighting_vertex, shader_capabilities<camera, light_handler>> custom_shader() {
        shader<lighting_vertex, shader_capabilities<camera, light_handler>>
            ourShader(DEFAULT_VERTEX_SHADER,
                      DEFAULT_FRAGMENT_SHADER,
                      {{{0},
                        shader_input::vec3_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, location) + offsetof(lighting_vertex::location_t, value)},
                        {sizeof(lighting_vertex)}},
                       {{1},
                        shader_input::vec2_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, texture) + offsetof(lighting_vertex::texture_t, coord)},
                        {sizeof(lighting_vertex)}},
                       {{2},
                        shader_input::int_type,
                        shader_input_storage_type::signed_int,
                        {offsetof(lighting_vertex, texture) + offsetof(lighting_vertex::texture_t, layer)},
                        {sizeof(lighting_vertex)}},
                       {{3}, shader_input::vec3_type, shader_input_storage_type::floating_point, {offsetof(lighting_vertex, normal)}, {sizeof(lighting_vertex)}},
                       {{4},
                        shader_input::vec3_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, material) + offsetof(lighting_vertex::material_t, ambient)},
                        {sizeof(lighting_vertex)}},
                       {{5},
                        shader_input::vec3_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, material) + offsetof(lighting_vertex::material_t, diffuse)},
                        {sizeof(lighting_vertex)}},
                       {{6},
                        shader_input::vec3_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, material) + offsetof(lighting_vertex::material_t, specular)},
                        {sizeof(lighting_vertex)}},
                       {{7},
                        shader_input::float_type,
                        shader_input_storage_type::floating_point,
                        {offsetof(lighting_vertex, material) + offsetof(lighting_vertex::material_t, shininess)},
                        {sizeof(lighting_vertex)}}});

        auto uniforms = ourShader.uniforms();

        uniforms.set_mat4("camera", glm::mat4{1.0f});

        return ourShader;
    }
}    // namespace basic_game
