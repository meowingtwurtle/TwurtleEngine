#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "randomcat/engine/low_level/graphics/color.hpp"
#include "randomcat/engine/low_level/graphics/detail/gl_error_guard.hpp"

namespace randomcat::engine::graphics {
    inline void set_global_background_color(color_rgb _color) noexcept {
        RC_GL_ERROR_GUARD("setting global background color");

        glClearColor(_color.r, _color.g, _color.b, 1.0);
    }

    inline void clear_graphics() noexcept {
        RC_GL_ERROR_GUARD("clearing screen");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void enable_depth_test() noexcept {
        RC_GL_ERROR_GUARD("enabling depth test");

        glEnable(GL_DEPTH_TEST);
    }
}    // namespace randomcat::engine::graphics
