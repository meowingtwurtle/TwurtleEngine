#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "randomcat/engine/graphics/texture_array_index.hpp"

namespace randomcat::engine::graphics {
    struct default_vertex {
        glm::vec3 location;
        glm::vec2 texCoord;
        texture_array_index layerNum;
    };
}    // namespace randomcat::engine::graphics
