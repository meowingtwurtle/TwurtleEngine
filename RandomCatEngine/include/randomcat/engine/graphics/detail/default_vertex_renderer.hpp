#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include <randomcat/engine/graphics/detail/gl_error_guard.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.hpp>
#include <randomcat/engine/graphics/shader.hpp>

// I must put definitions here because of stupid C++ template rules.

namespace randomcat::engine::graphics::detail {
    template<typename Vertex>
    class vertex_renderer {
    public:
        using vertex = Vertex;

        vertex_renderer(vertex_renderer const&) = delete;
        vertex_renderer(vertex_renderer&&) noexcept = default;

        explicit vertex_renderer(shader_view<vertex> _shader) noexcept : m_shader(std::move(_shader)) {
            RC_GL_ERROR_GUARD("initializing vertex renderer");

            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            for (shader_input input : m_shader.inputs()) {
                glVertexAttribPointer(input.position, input.count, input.type, input.normalized, input.stride, input.offset);
                glEnableVertexAttribArray(input.position);
            }
        }

        template<typename T>
        void operator()(T const& _t) const noexcept(false) {
            std::optional<active_lock> lock = std::nullopt;
            if (!is_forced_active()) lock.emplace(*this);

            render_active(_t);
        }

        // Must not outlive the renderer object
        class active_lock {
        public:
            active_lock(active_lock const&) = delete;
            active_lock(active_lock&&) = delete;

            active_lock(vertex_renderer const& _renderer) noexcept(false) : m_renderer(_renderer) {
                m_renderer.make_active();
                m_renderer.set_forced_active();
            }

            ~active_lock() noexcept { m_renderer.release_forced_active(); }

        private:
            vertex_renderer const& m_renderer;
        };

        active_lock make_active_lock() const noexcept(false) {
            return active_lock(*this);    // Constructor activates the renderer
        }

    private:
        template<typename T0, typename T1, typename = std::enable_if_t<std::is_pointer_v<T0> && std::is_pointer_v<T1>>>
        struct is_pointer_same :
        std::bool_constant<std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T0>>, std::remove_cv_t<std::remove_pointer_t<T1>>>> {};

        template<typename T0, typename T1>
        static auto constexpr is_pointer_same_v = is_pointer_same<T0, T1>::value;

        template<typename container>
        static auto constexpr is_vertex_container_v = is_pointer_same_v<decltype(std::declval<container const&>().data()), vertex*>;

        template<bool V>
        using bool_if_t = std::enable_if_t<V, bool>;

        void make_active() const noexcept {
            RC_GL_ERROR_GUARD("activating vertex renderer");

            m_shader.make_active();
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

        // bool_if_t makes the template parameter lists different here, so no
        // redeclaration If this is confusing, go watch
        // https://www.youtube.com/watch?v=ybaE9qlhHvw bool_if_t evaluates to bool
        // only if the value is true, otherwise it SFINAEs away

        // is_vertex_container_v tests if we can use this as a vertex container
        // (namely, if it has a possibly data() method returning a cv-pointer to
        // vertex)

        template<typename _container_t, bool_if_t<is_vertex_container_v<_container_t>> = true>
        void render_active(_container_t const& _vertices) const noexcept {
            RC_GL_ERROR_GUARD("vertex renderer rendering");

            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex), _vertices.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        }

        template<typename T, bool_if_t<!is_vertex_container_v<T>> = true>
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
        unique_vao_id m_vao;
        unique_vbo_id m_vbo;
        shader_view<vertex> m_shader;
    };

    using default_vertex_renderer = vertex_renderer<detail::default_vertex>;
}    // namespace randomcat::engine::graphics::detail
