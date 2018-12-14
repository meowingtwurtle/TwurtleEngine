#include <randomcat/engine/graphics/camera.hpp>

namespace randomcat::engine::graphics {
    void camera::update_shader() noexcept {
        glm::mat4 view = glm::lookAt(m_pos, m_pos + m_dir, glm::vec3{0.0f, 1.0f, 0.0f});
        glm::mat4 projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_minDistance, m_maxDistance);
        m_uniforms.set_mat4("camera", projection * view);
    }
}    // namespace randomcat::engine::graphics
