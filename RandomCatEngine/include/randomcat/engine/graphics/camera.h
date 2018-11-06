#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    class camera {
    public:
        explicit camera(graphics::shader _shader) noexcept : m_shader(std::move(_shader)) {}
        camera(graphics::shader _shader, glm::vec3 _pos) noexcept : m_shader(std::move(_shader)), m_pos(_pos) {}
        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir) noexcept : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov) noexcept
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio) noexcept
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio, float _shortDistance, float _longDistance) noexcept
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio), m_minDistance(_shortDistance), m_maxDistance(_longDistance) {
            update_shader();
        }

        graphics::shader const& shader() const noexcept { return m_shader; }

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

        graphics::shader m_shader;

        glm::vec3 m_pos{0.0f};
        glm::vec3 m_dir{0.0f};
        float m_fov = 45.0f;
        float m_aspectRatio = 1.0f;
        float m_minDistance = 0.1f;
        float m_maxDistance = 100.0f;
    };
}    // namespace randomcat::engine::graphics
