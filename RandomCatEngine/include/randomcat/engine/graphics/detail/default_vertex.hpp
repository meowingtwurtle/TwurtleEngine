#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "randomcat/engine/graphics/texture_array_index.hpp"

namespace randomcat::engine::graphics {
    struct default_vertex {
        struct location {
            glm::vec3 value;
        } loc;

        struct texture {
            glm::vec2 coord;
            texture_array_index layer;
        } tex;

        glm::vec3 normal;
    };
}    // namespace randomcat::engine::graphics
