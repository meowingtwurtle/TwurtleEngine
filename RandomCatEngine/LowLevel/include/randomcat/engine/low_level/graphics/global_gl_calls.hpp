#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "randomcat/engine/low_level/graphics/color.hpp"

namespace randomcat::engine::graphics {
    inline void set_global_background_color(color_rgb _color) noexcept { glClearColor(_color.r, _color.g, _color.b, 1.0); }

    inline void clear_graphics() noexcept { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    inline void enable_depth_test() noexcept { glEnable(GL_DEPTH_TEST); }
}    // namespace randomcat::engine::graphics
