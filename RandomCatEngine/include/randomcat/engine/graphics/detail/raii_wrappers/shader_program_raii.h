#pragma once

#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/opengl_raii_id.h>

namespace randomcat::engine::graphics::detail {
    inline opengl_raw_id makeShader(GLenum _type) noexcept { return opengl_raw_id{glCreateShader(_type)}; }

    inline void destroyShader(opengl_raw_id _id) noexcept { glDeleteShader(_id); }

    inline opengl_raw_id makeProgram() noexcept { return opengl_raw_id{glCreateProgram()}; }

    inline void destroyProgram(opengl_raw_id _id) noexcept { glDeleteProgram(_id); }

    using shader_id = opengl_raii_id<makeShader, destroyShader, GLenum>;
    using program_id = opengl_raii_id<makeProgram, destroyProgram>;
}    // namespace randomcat::engine::graphics::detail
