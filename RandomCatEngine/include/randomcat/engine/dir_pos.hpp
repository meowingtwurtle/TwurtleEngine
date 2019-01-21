#pragma once

#include <glm/glm.hpp>

namespace randomcat::engine {
    struct dir_yaw_pitch {
        float yaw;
        float pitch;
    };

    struct dir_facing {
        float x;
        float y;
        float z;
    };

    struct pos_components {
        float x;
        float y;
        float z;
    };

    inline glm::vec3 as_glm(dir_facing d) noexcept { return {d.x, d.y, d.z}; }

    inline dir_facing as_components(dir_yaw_pitch d) noexcept {
        auto x = static_cast<float>(cos(glm::radians(d.pitch)) * cos(glm::radians(d.yaw)));
        auto y = static_cast<float>(sin(glm::radians(d.pitch)));
        auto z = static_cast<float>(cos(glm::radians(d.pitch)) * sin(glm::radians(d.yaw)));

        return {x, y, z};
    }

    inline glm::vec3 as_glm(dir_yaw_pitch d) noexcept { return as_glm(as_components(d)); }

    inline glm::vec3 as_glm(pos_components p) noexcept { return {p.x, p.y, p.z}; }

    class position {
    public:
        /* implicit */ position(pos_components _pos) noexcept : m_pos(_pos) {}
        explicit position(glm::vec3 _pos) noexcept : m_pos{_pos.x, _pos.y, _pos.z} {}

        /* implicit */ operator pos_components() const noexcept { return components(); }
        pos_components components() const noexcept { return m_pos; }

        float x() const noexcept { return m_pos.x; }
        float y() const noexcept { return m_pos.y; }
        float z() const noexcept { return m_pos.z; }

        glm::vec3 as_glm() const noexcept { return engine::as_glm(m_pos); }

    private:
        pos_components m_pos;
    };

    inline glm::vec3 as_glm(position _pos) noexcept { return _pos.as_glm(); }

    class direction {
    public:
        /* implicit */ direction(dir_facing _dir) noexcept : m_dir(std::move(_dir)) {}
        /* implicit */ direction(dir_yaw_pitch _dir) noexcept : m_dir(engine::as_components(_dir)) {}
        explicit direction(glm::vec3 _dir) noexcept : m_dir{.x = _dir.x, .y = _dir.y, .z = _dir.z} {}

        /* implicit */ operator dir_facing() const noexcept { return components(); }
        dir_facing components() const noexcept { return m_dir; }

        glm::vec3 as_glm() const noexcept { return engine::as_glm(m_dir); }

        float facing_x() const noexcept { return m_dir.x; }
        float facing_y() const noexcept { return m_dir.y; }
        float facing_z() const noexcept { return m_dir.z; }

        /* implicit */ operator dir_yaw_pitch() const noexcept { return yaw_pitch(); }

        dir_yaw_pitch yaw_pitch() const noexcept {
            auto pitch = glm::asin(facing_y());
            ;
            auto cosPitch = glm::cos(pitch);

            auto yaw = glm::atan(facing_z() / cosPitch, facing_x() / cosPitch);

            return {.yaw = yaw, .pitch = pitch};
        }

        float yaw() const noexcept { return yaw_pitch().yaw; }

        float pitch() const noexcept { return yaw_pitch().pitch; }

    private:
        dir_facing m_dir;
    };

    inline glm::vec3 as_glm(direction _dir) noexcept { return _dir.as_glm(); }
}    // namespace randomcat::engine
