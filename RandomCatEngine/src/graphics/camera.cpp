#include <randomcat/engine/graphics/camera.h>

namespace randomcat::engine::graphics {
    void camera::set_dir(float _yaw, float _pitch) noexcept {
        m_dir.x = static_cast<float>(cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)));
        m_dir.y = static_cast<float>(sin(glm::radians(_pitch)));
        m_dir.z = static_cast<float>(cos(glm::radians(_pitch)) * sin(glm::radians(_yaw)));
        update_shader();
    }

    void camera::update_shader() noexcept {
        m_uniforms.set_mat4("view", glm::lookAt(m_pos, m_pos + m_dir, glm::vec3{0.0f, 1.0f, 0.0f}));
        m_uniforms.set_mat4("projection", glm::perspective(glm::radians(m_fov), m_aspectRatio, m_minDistance, m_maxDistance));
    }
}    // namespace randomcat::engine::graphics
