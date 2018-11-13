#pragma once

#include <functional>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    class camera {
    public:
        camera(shader::uniform_manager _uniforms, glm::vec3 _pos = glm::vec3{0.0f}, glm::vec3 _dir = glm::vec3{0.0f}, float _fov = 45.0f, float _aspectRatio = 1.0f) noexcept
        : camera(std::move(_uniforms), std::move(_pos), std::move(_dir), std::move(_fov), std::move(_aspectRatio), 0.1f, 100.0f) {}

        camera(shader::uniform_manager _uniforms, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio, float _shortDistance, float _longDistance) noexcept
        : m_uniforms(std::move(_uniforms)),
          m_pos(_pos),
          m_dir(_dir),
          m_fov(_fov),
          m_aspectRatio(_aspectRatio),
          m_minDistance(_shortDistance),
          m_maxDistance(_longDistance) {
            update_shader();
        }

        glm::vec3 pos() const noexcept { return m_pos; }
        void set_pos(glm::vec3 _pos) noexcept {
            m_pos = _pos;
            update_shader();
        }
        void add_pos(glm::vec3 _delta) noexcept {
            m_pos += _delta;
            update_shader();
        }

        glm::vec3 dir() const noexcept { return m_dir; }
        void set_dir(glm::vec3 _dir) noexcept {
            m_dir = _dir;
            update_shader();
        }
        void add_dir(glm::vec3 _delta) noexcept {
            m_dir += _delta;
            update_shader();
        }

        void set_dir(float _yaw, float _pitch) noexcept;

        float fov() const noexcept { return m_fov; }
        void set_fov(float _fov) noexcept {
            m_fov = _fov;
            update_shader();
        }

        float min_distance() const noexcept { return m_minDistance; }
        void set_min_distance(float _shortDistance) noexcept {
            m_minDistance = _shortDistance;
            update_shader();
        }

        float max_distance() const noexcept { return m_maxDistance; }
        void set_max_distance(float _longDistance) noexcept {
            m_maxDistance = _longDistance;
            update_shader();
        }

    private:
        void update_shader() noexcept;

        shader::uniform_manager m_uniforms;

        glm::vec3 m_pos;
        glm::vec3 m_dir;
        float m_fov;
        float m_aspectRatio;
        float m_minDistance;
        float m_maxDistance;
    };
}    // namespace randomcat::engine::graphics
