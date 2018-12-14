#pragma once

#include <glm/glm.hpp>

namespace randomcat::engine {
    struct dir_yaw_pitch {
        float yaw;
        float pitch;
    };

    struct dir_components {
        float x;
        float y;
        float z;
    };

    struct pos_components {
        float x;
        float y;
        float z;
    };

    struct pos_and_dir_yp {
        pos_components pos;
        dir_yaw_pitch dir;
    };

    struct pos_and_dir_comp {
        pos_components pos;
        dir_components dir;
    };

    inline static constexpr pos_components pos(glm::vec3 vec) noexcept { return {vec.x, vec.y, vec.z}; }

    inline static constexpr dir_components dir(glm::vec3 dir) noexcept { return {dir.x, dir.y, dir.z}; }

    inline static constexpr pos_components pos(float x, float y, float z) noexcept { return {x, y, z}; }

    inline static constexpr dir_components dir(float x, float y, float z) noexcept { return {x, y, z}; }

    // Identity functions - constrain types and help express intent without naming
    // types

    inline static constexpr pos_components pos(pos_components p) noexcept { return p; }

    inline static constexpr dir_components dir(dir_components d) noexcept { return d; }

    inline static constexpr dir_yaw_pitch dir(dir_yaw_pitch d) noexcept { return d; }

    // Conversion functions - help express intent

    inline static glm::vec3 as_glm(dir_components d) noexcept { return {d.x, d.y, d.z}; }

    inline static dir_components as_components(dir_yaw_pitch d) noexcept {
        auto x = static_cast<float>(cos(glm::radians(d.pitch)) * cos(glm::radians(d.yaw)));
        auto y = static_cast<float>(sin(glm::radians(d.pitch)));
        auto z = static_cast<float>(cos(glm::radians(d.pitch)) * sin(glm::radians(d.yaw)));

        return {x, y, z};
    }

    inline static glm::vec3 as_glm(dir_yaw_pitch d) noexcept { return as_glm(as_components(d)); }

    inline static glm::vec3 as_glm(pos_components p) noexcept { return {p.x, p.y, p.z}; }
}    // namespace randomcat::engine
