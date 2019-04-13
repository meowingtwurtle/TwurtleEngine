#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "randomcat/engine/low_level/graphics/color.hpp"
#include "randomcat/engine/low_level/graphics/shader_uniforms.hpp"

namespace randomcat::engine::graphics {
    class light_handler {
    public:
        struct light_t {
            glm::vec3 position;

            struct colors_t {
                color_rgb ambient;
                color_rgb diffuse;
                color_rgb specular;
            } colors;

            struct attenuation_t {
                GLfloat constant;
                GLfloat linear;
                GLfloat quadratic;
            } attenuation;
        };

        explicit light_handler(shader_uniform_writer<uniform_capabilities<light_handler>> _uniformWriter) noexcept
        : m_uniformWriter(std::move(_uniformWriter)) {}

        void add_light(light_t _light) noexcept { m_lights.push_back(std::move(_light)); }
        void clear_lights() noexcept { m_lights.clear(); }

        void update() const noexcept;

    private:
        std::vector<light_t> m_lights;
        shader_uniform_writer<uniform_capabilities<light_handler>> m_uniformWriter;
    };
}    // namespace randomcat::engine::graphics
