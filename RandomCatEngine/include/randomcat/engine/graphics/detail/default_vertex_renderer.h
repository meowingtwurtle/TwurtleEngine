#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include <randomcat/engine/detail/noexcept_util.h>
#include <randomcat/engine/detail/tag.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.h>
#include <randomcat/engine/graphics/shader.h>

// I must put definitions here because of stupid C++ template rules.

namespace randomcat::engine::graphics::detail {
    template<typename _vertex_t>
    class default_vertex_renderer {
    public:
        default_vertex_renderer(randomcat::engine::graphics::shader _shader) noexcept : m_shader(std::move(_shader)) {
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            for (shader_input input : m_shader.inputs()) {
                glVertexAttribPointer(input.position, input.count, input.type, input.normalized, input.stride, input.offset);
                glEnableVertexAttribArray(input.position);
            }
        }

        default_vertex_renderer(tag_t<_vertex_t>, shader _shader) noexcept : default_vertex_renderer(std::move(_shader)) {}

        RC_NOEXCEPT_CONSTRUCT_ASSIGN(default_vertex_renderer);

        using vertex = _vertex_t;
        using container = std::vector<vertex>;

        void operator()(container const& _vertices) const noexcept {
            if (!is_forced_active()) make_active();
            render_active(_vertices);
        }

        template<typename T>
        void operator()(T const& _t) const noexcept {
            if (!is_forced_active()) make_active();
            render_active<T>(_t);
        }

        // Must not outlive the renderer object
        class active_lock {
        public:
            active_lock(active_lock const&) = delete;
            active_lock(active_lock&&) = delete;

            active_lock(default_vertex_renderer const& _renderer) noexcept : m_renderer(_renderer) {
                m_renderer.make_active();
                m_renderer.set_forced_active();
            }

            ~active_lock() noexcept { m_renderer.release_forced_active(); }

        private:
            default_vertex_renderer const& m_renderer;
        };

        active_lock make_active_lock() const noexcept {
            return active_lock(*this);    // Constructor activates the renderer
        }

    private:
        void make_active() const noexcept {
            m_shader.make_active();
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

        void render_active(container const& _vertices) const noexcept {
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertex_t), _vertices.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        }

        template<typename T>
        void render_active(T const& _t) const noexcept {
            std::for_each(begin(_t), end(_t), [this](auto const& x) { render_active(x); });
        }

        void set_forced_active() const noexcept(false) {
            if (is_forced_active()) throw std::runtime_error("Cannot double-force active.");
            m_isForcedActive = true;
        }

        void release_forced_active() const noexcept { m_isForcedActive = false; }

        bool is_forced_active() const noexcept { return m_isForcedActive; }

        mutable bool m_isForcedActive = false;
        vao_id m_vao;
        vbo_id m_vbo;
        randomcat::engine::graphics::shader m_shader;
    };
}    // namespace randomcat::engine::graphics::detail
