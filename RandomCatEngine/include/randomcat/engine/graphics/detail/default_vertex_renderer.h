#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.h>
#include <randomcat/engine/graphics/shader.h>

// I must put definitions here because of stupid C++ template rules.

namespace randomcat::engine::graphics::detail {
    template<typename _vertex_t>
    class default_vertex_renderer {
    public:
        default_vertex_renderer(randomcat::engine::graphics::shader _shader) : m_shader(std::move(_shader)) {
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            for (shader_input input : m_shader.inputs()) {
                glVertexAttribPointer(input.position, input.count, input.type, input.normalized, input.stride, input.offset);
                glEnableVertexAttribArray(input.position);
            }
        }

        template<typename... Ts>
        default_vertex_renderer(tag<_vertex_t>, Ts... _args) : default_vertex_renderer(_args...) {}
        using vertex = _vertex_t;
        using container = detail::default_renderer_container<vertex>;
        using container_container = detail::default_renderer_container<container>;

        void operator()(container const& _vertices) const {
            makeActive();
            renderActive(_vertices);
        }

        template<typename T>
        void operator()(T const& _t) const {
            makeActive();
            renderActive<T>(_t);
        }

    private:
        void makeActive() const {
            m_shader.make_active();
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

        void renderActive(container const& _vertices) const {
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertex_t), _vertices.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        }

        template<typename T>
        void renderActive(const T& _t) const {
            std::for_each(begin(_t), end(_t), [this](const auto& x) { renderActive(x); });
        }

        vao_id m_vao;
        vbo_id m_vbo;
        randomcat::engine::graphics::shader m_shader;
    };
}    // namespace randomcat::engine::graphics::detail
