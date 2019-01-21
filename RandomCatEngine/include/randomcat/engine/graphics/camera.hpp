#pragma once

#include <functional>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <randomcat/engine/dir_pos.hpp>
#include <randomcat/engine/graphics/detail/default_vertex.hpp>
#include <randomcat/engine/graphics/shader.hpp>

namespace randomcat::engine::graphics {
    struct camera_state {
        float aspectRatio;
        float fov;
        float minDistance;
        float maxDistance;
        direction dir;
        position pos;
    };

    // This is a UniformCapability.
    class camera {
    public:
        // State is indeterminate until update_state is called
        explicit camera(shader_uniform_manager _uniforms) noexcept : m_uniforms(std::move(_uniforms)) {}

        void update(camera_state const& _state) noexcept;

        static shader<default_vertex, shader_capabilities<camera>> camera_shader();

    private:
        shader_uniform_manager m_uniforms;
    };
}    // namespace randomcat::engine::graphics
