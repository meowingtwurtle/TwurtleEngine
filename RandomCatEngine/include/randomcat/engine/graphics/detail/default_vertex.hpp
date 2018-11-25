#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace randomcat::engine::graphics::detail {
    struct default_vertex {
        glm::vec3 location;
        glm::vec2 texCoord;
        unsigned int layerNum;
    };
}    // namespace randomcat::engine::graphics::detail
