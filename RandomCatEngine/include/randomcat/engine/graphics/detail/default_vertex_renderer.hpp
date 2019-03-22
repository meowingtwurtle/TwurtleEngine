#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/vao_raii.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.hpp"
#include "randomcat/engine/graphics/shader.hpp"

// I must put definitions here because of stupid C++ template rules.

namespace randomcat::engine::graphics {
    struct vertex_renderer_double_lock_error : std::exception {
        char const* what() const noexcept override { return "Cannot double lock vertex renderer"; }
    };

    template<typename Vertex>
    class vertex_renderer {
    public:
        using vertex = Vertex;

        vertex_renderer(vertex_renderer const&) = delete;
        vertex_renderer(vertex_renderer&&) noexcept = delete;

        explicit vertex_renderer(shader_view<vertex> _shader) noexcept : m_shader(std::move(_shader)) {
            RC_GL_ERROR_GUARD("initializing vertex renderer");

            glBindVertexArray(m_vao.value());
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());

            for (shader_input input : m_shader.inputs()) {
                auto const rawStorageType = static_cast<GLenum>(input.storageType);
                auto const rawOffset = reinterpret_cast<void*>(input.offset.value);

                switch (input.attributeType.base()) {
                    case shader_input_attribute_base_type::floating_point: {
                        glVertexAttribPointer(input.index.value, input.attributeType.size().value, rawStorageType, false, input.stride.value, rawOffset);

                        break;
                    }

                    case shader_input_attribute_base_type::integral: {
                        glVertexAttribIPointer(input.index.value, input.attributeType.size().value, rawStorageType, input.stride.value, rawOffset);

                        break;
                    }
                }

                glEnableVertexAttribArray(input.index.value);
            }
        }

        template<typename T>
        void operator()(T const& _t) const noexcept {
            auto l = make_active_lock();
            render_active(_t);
        }

    private:
        struct active_lock_held {};

    public:
        // Must not outlive the renderer object
        class active_lock {
        public:
            active_lock(active_lock const&) = delete;
            active_lock(active_lock&&) = delete;

            active_lock(vertex_renderer const& _renderer) noexcept : m_renderer(_renderer) {
                m_renderer.make_active();
                if (_renderer.m_activeLock.expired()) {
                    auto newHeld = std::make_shared<active_lock_held>();
                    _renderer.m_activeLock = newHeld;
                    m_holdLock = std::move(newHeld);
                } else {
                    m_holdLock = _renderer.m_activeLock.lock();
                }
            }

        private:
            std::shared_ptr<active_lock_held> m_holdLock;
            vertex_renderer const& m_renderer;
        };

        active_lock make_active_lock() const noexcept {
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
            glBindVertexArray(m_vao.value());
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        }

        template<typename _container_t, typename = std::enable_if_t<is_vertex_container_v<_container_t>>>
        void render_active(_container_t const& _vertices) const noexcept {
            RC_GL_ERROR_GUARD("vertex renderer rendering");

            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex), _vertices.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        }

        gl_detail::unique_vao_id m_vao;
        gl_detail::unique_vbo_id m_vbo;
        shader_view<vertex> m_shader;

        mutable std::weak_ptr<active_lock_held> m_activeLock = std::weak_ptr<active_lock_held>();
    };

    using default_vertex_renderer = vertex_renderer<default_vertex>;
}    // namespace randomcat::engine::graphics
