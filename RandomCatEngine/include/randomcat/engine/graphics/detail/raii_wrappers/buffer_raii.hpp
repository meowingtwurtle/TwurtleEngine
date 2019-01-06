#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.hpp>

namespace randomcat::engine::graphics::detail {
    template<typename Tag>
    inline decltype(auto) makeBuffer() noexcept {
        unsigned id;
        glGenBuffers(1, &id);
        return opengl_raw_id{id};
    }

    template<typename Tag>
    inline void destroyBuffer(opengl_raw_id _id) noexcept {
        glDeleteBuffers(1, &_id);
    }

    template<typename Tag>
    using buffer_id = opengl_raii_id<makeBuffer<Tag>, destroyBuffer<Tag>>;
}    // namespace randomcat::engine::graphics::detail
