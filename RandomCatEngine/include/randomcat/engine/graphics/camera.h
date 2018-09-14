#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    class camera {
    public:
        explicit camera(graphics::shader _shader) : m_shader(std::move(_shader)) {}
        camera(graphics::shader _shader, glm::vec3 _pos) : m_shader(std::move(_shader)), m_pos(_pos) {}
        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir) : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio) {
            update_shader();
        }

        camera(graphics::shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio, float _shortDistance, float _longDistance)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio), m_minDistance(_shortDistance), m_maxDistance(_longDistance) {
            update_shader();
        }

        graphics::shader const& shader() const& { return m_shader; }

        glm::vec3 pos() const { return m_pos; }
        void set_pos(glm::vec3 _pos) {
            m_pos = _pos;
            update_shader();
        }
        void add_pos(glm::vec3 _delta) {
            m_pos += _delta;
            update_shader();
        }

        glm::vec3 dir() const { return m_dir; }
        void set_dir(glm::vec3 _dir) {
            m_dir = _dir;
            update_shader();
        }
        void add_dir(glm::vec3 _delta) {
            m_dir += _delta;
            update_shader();
        }

        void set_dir(float _yaw, float _pitch);

        float fov() const { return m_fov; }
        void set_fov(float _fov) {
            m_fov = _fov;
            update_shader();
        }

        float min_distance() const { return m_minDistance; }
        void set_min_distance(float _shortDistance) {
            m_minDistance = _shortDistance;
            update_shader();
        }

        float max_distance() const { return m_maxDistance; }
        void set_max_distance(float _longDistance) {
            m_maxDistance = _longDistance;
            update_shader();
        }

    private:
        void update_shader();

        graphics::shader m_shader;

        glm::vec3 m_pos{0.0f};
        glm::vec3 m_dir{0.0f};
        float m_fov = 45.0f;
        float m_aspectRatio = 1.0f;
        float m_minDistance = 0.1f;
        float m_maxDistance = 100.0f;
    };
}    // namespace randomcat::engine::graphics
