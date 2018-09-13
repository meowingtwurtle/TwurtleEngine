#include <randomcat/engine/graphics/camera.h>

namespace randomcat::engine::graphics {
    void camera::setDir(float _yaw, float _pitch) {
        m_dir.x = static_cast<float>(cos(glm::radians(_pitch)) * cos(glm::radians(_yaw)));
        m_dir.y = static_cast<float>(sin(glm::radians(_pitch)));
        m_dir.z = static_cast<float>(cos(glm::radians(_pitch)) * sin(glm::radians(_yaw)));
        updateShader();
    }

    void camera::updateShader() {
        m_shader.setMat4("view", glm::lookAt(m_pos, m_pos + m_dir, glm::vec3{0.0f, 1.0f, 0.0f}));
        m_shader.setMat4("projection", glm::perspective(glm::radians(m_fov), m_aspectRatio, m_shortDistance, m_longDistance));
    }
}    // namespace randomcat::engine::graphics
