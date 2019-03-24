#pragma once

#include "randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/texture_raii.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/vao_raii.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.hpp"

namespace randomcat::engine::graphics::gl_detail {
    void activate_vao(raw_vao_id _vao) noexcept;
    [[nodiscard]] raw_vao_id current_vao() noexcept;

    void activate_vbo(raw_vbo_id _vbo) noexcept;
    [[nodiscard]] raw_vbo_id current_vbo() noexcept;

    void activate_program(raw_program_id _program) noexcept;
    [[nodiscard]] raw_program_id current_program() noexcept;

    namespace impl {
        template<auto Activate, auto Current>
        class gl_lock;

        template<typename IdType, void (*Activate)(IdType) noexcept, IdType (*Current)() noexcept>
        class gl_lock<Activate, Current> {
        public:
            static_assert(std::is_trivial_v<IdType>);

            gl_lock(gl_lock const&) = delete;
            gl_lock(gl_lock&&) = delete;

            gl_lock(IdType _new) noexcept : m_old(Current()) { Activate(_new); }

            ~gl_lock() noexcept { Activate(m_old); }

        private:
            IdType m_old;
        };
    }    // namespace impl

    using vao_lock = impl::gl_lock<activate_vao, current_vao>;
    using vbo_lock = impl::gl_lock<activate_vbo, current_vbo>;
    using program_lock = impl::gl_lock<activate_program, current_program>;
}    // namespace randomcat::engine::graphics::gl_detail
