#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.h>

namespace randomcat::engine::graphics::detail {
    inline decltype(auto) makeVBO() noexcept {
        unsigned id;
        glGenBuffers(1, &id);
        return opengl_raw_id{id};
    }

    inline void destroyVBO(opengl_raw_id _id) noexcept { glDeleteBuffers(1, &_id); }

    using vbo_id = opengl_raii_id<makeVBO, destroyVBO>;
}    // namespace randomcat::engine::graphics::detail
