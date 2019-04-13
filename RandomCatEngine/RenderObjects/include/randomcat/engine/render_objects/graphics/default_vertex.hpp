#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "randomcat/engine/low_level/graphics/vertex_renderer.hpp"
#include "randomcat/engine/textures/graphics/texture_array_index.hpp"

namespace randomcat::engine::graphics {
    struct default_vertex {
        struct location_t {
            glm::vec3 value;
        } location;

        struct texture_t {
            glm::vec2 coord;
            texture_array_index layer;
        } texture;

        glm::vec3 normal;
    };

    using default_vertex_renderer = vertex_renderer<default_vertex>;
}    // namespace randomcat::engine::graphics
