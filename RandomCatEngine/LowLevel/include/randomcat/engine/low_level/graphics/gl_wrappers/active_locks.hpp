#pragma once

#include "randomcat/engine/low_level/detail/raii_active_lock.hpp"
#include "randomcat/engine/low_level/graphics/gl_wrappers/shader_program_raii.hpp"
#include "randomcat/engine/low_level/graphics/gl_wrappers/texture_raii.hpp"
#include "randomcat/engine/low_level/graphics/gl_wrappers/vao_raii.hpp"
#include "randomcat/engine/low_level/graphics/gl_wrappers/vbo_raii.hpp"

namespace randomcat::engine::graphics::gl_detail {
    void activate_vao(raw_vao_id _vao) noexcept;
    [[nodiscard]] raw_vao_id current_vao() noexcept;

    void activate_vbo(raw_vbo_id _vbo) noexcept;
    [[nodiscard]] raw_vbo_id current_vbo() noexcept;

    void activate_program(raw_program_id _program) noexcept;
    [[nodiscard]] raw_program_id current_program() noexcept;

    using vao_lock = util_detail::basic_active_lock<activate_vao, current_vao>;
    using vbo_lock = util_detail::basic_active_lock<activate_vbo, current_vbo>;
    using program_lock = util_detail::basic_active_lock<activate_program, current_program>;
}    // namespace randomcat::engine::graphics::gl_detail
