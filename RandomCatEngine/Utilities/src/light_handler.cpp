#include "randomcat/engine/low_level/graphics/shader.hpp"
#include "randomcat/engine/utilities/graphics/lights.hpp"

namespace randomcat::engine::graphics {
    void light_handler::update() const noexcept {
        m_uniformWriter.set_int("lightsUsed", m_lights.size());

        for (std::uint16_t i = 0; i < m_lights.size(); ++i) {
            auto prefix = "lights[" + std::to_string(i) + "]";
            auto const& light = m_lights[i];

            m_uniformWriter.set_vec3(prefix + ".position", light.position);
            m_uniformWriter.set_vec3(prefix + ".ambient", light.colors.ambient.as_glm());
            m_uniformWriter.set_vec3(prefix + ".diffuse", light.colors.diffuse.as_glm());
            m_uniformWriter.set_vec3(prefix + ".specular", light.colors.specular.as_glm());
            m_uniformWriter.set_float(prefix + ".constant", light.attenuation.constant);
            m_uniformWriter.set_float(prefix + ".linear", light.attenuation.linear);
            m_uniformWriter.set_float(prefix + ".quadratic", light.attenuation.quadratic);
        }
    }
}    // namespace randomcat::engine::graphics
