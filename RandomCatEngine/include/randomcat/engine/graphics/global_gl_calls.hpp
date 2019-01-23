#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"

namespace randomcat::engine::graphics {
    /// Floats should be between 0 and 1
    inline void set_global_background_color(float _red, float _green, float _blue, float _alpha) noexcept {
        RC_GL_ERROR_GUARD("setting global background color");

        glClearColor(_red, _green, _blue, _alpha);
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
