#pragma once

#include <functional>

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "randomcat/engine/dir_pos.hpp"
#include "randomcat/engine/graphics/detail/default_vertex.hpp"
#include "randomcat/engine/graphics/shader.hpp"

namespace randomcat::engine::graphics {
    struct camera_state {
        GLfloat aspectRatio;
        units::quantity<double, units::radians_unit> fov;
        GLfloat minDistance;
        GLfloat maxDistance;
        direction dir;
        position pos;
    };

    // This is a UniformCapability.
    class camera {
    public:
        // State is indeterminate until update_state is called
        explicit camera(shader_uniform_writer<shader_capabilities<camera>> _uniforms) noexcept : m_uniforms(std::move(_uniforms)) {}

        void update(camera_state const& _state) noexcept;

        [[nodiscard]] static shader<default_vertex, shader_capabilities<camera>> camera_shader();

    private:
        shader_uniform_writer<uniform_capabilities<camera>> m_uniforms;
    };
}    // namespace randomcat::engine::graphics
