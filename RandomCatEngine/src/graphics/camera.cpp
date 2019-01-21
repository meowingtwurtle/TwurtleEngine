#include <randomcat/engine/graphics/camera.hpp>

namespace randomcat::engine::graphics {
    void camera::update(camera_state const& _state) noexcept {
        glm::mat4 view = glm::lookAt(as_glm(_state.pos), as_glm(_state.pos) + as_glm(_state.dir), glm::vec3{0.0f, 1.0f, 0.0f});
        glm::mat4 projection = glm::perspective<double>(units::radians(_state.fov).count(), _state.aspectRatio, _state.minDistance, _state.maxDistance);

        //         noexcept guaranteed - we have guarantee that this uniform exists
        //         (or at least it should)
        m_uniforms.set_mat4("camera", projection * view);
    }
}    // namespace randomcat::engine::graphics
