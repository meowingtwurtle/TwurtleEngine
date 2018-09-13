#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    class camera {
    public:
        explicit camera(shader _shader) : m_shader(std::move(_shader)) {}
        camera(shader _shader, glm::vec3 _pos) : m_shader(std::move(_shader)), m_pos(_pos) {}
        camera(shader _shader, glm::vec3 _pos, glm::vec3 _dir) : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir) { updateShader(); }

        camera(shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov) {
            updateShader();
        }

        camera(shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio) {
            updateShader();
        }

        camera(shader _shader, glm::vec3 _pos, glm::vec3 _dir, float _fov, float _aspectRatio, float _shortDistance, float _longDistance)
        : m_shader(std::move(_shader)), m_pos(_pos), m_dir(_dir), m_fov(_fov), m_aspectRatio(_aspectRatio), m_shortDistance(_shortDistance), m_longDistance(_longDistance) {
            updateShader();
        }

        shader const& getShader() const& { return m_shader; }

        glm::vec3 pos() const { return m_pos; }
        void setPos(glm::vec3 _pos) {
            m_pos = _pos;
            updateShader();
        }
        void addPos(glm::vec3 _delta) {
            m_pos += _delta;
            updateShader();
        }

        glm::vec3 dir() const { return m_dir; }
        void setDir(glm::vec3 _dir) {
            m_dir = _dir;
            updateShader();
        }
        void addDir(glm::vec3 _delta) {
            m_dir += _delta;
            updateShader();
        }

        void setDir(float _yaw, float _pitch);

        float fov() const { return m_fov; }
        void setFov(float _fov) {
            m_fov = _fov;
            updateShader();
        }

        float shortDistance() const { return m_shortDistance; }
        void setShortDistance(float _shortDistance) {
            m_shortDistance = _shortDistance;
            updateShader();
        }

        float longDistance() const { return m_longDistance; }
        void setLongDistance(float _longDistance) {
            m_longDistance = _longDistance;
            updateShader();
        }

    private:
        void updateShader();

        shader m_shader;

        glm::vec3 m_pos{0.0f};
        glm::vec3 m_dir{0.0f};
        float m_fov = 45.0f;
        float m_aspectRatio = 1.0f;
        float m_shortDistance = 0.1f;
        float m_longDistance = 100.0f;
    };
}    // namespace randomcat::engine::graphics
