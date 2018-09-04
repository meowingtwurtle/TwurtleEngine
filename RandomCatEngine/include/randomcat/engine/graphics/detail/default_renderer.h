#pragma once

#include <type_traits>
#include <vector>
//#include <randomcat/engine/graphics/detail/raii_wrappers/texture_raii.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.h>
#include <randomcat/engine/graphics/renderer.h>
#include <randomcat/engine/graphics/shader.h>

// I must put definitions here because of stupid C++ template rules.

namespace randomcat::engine::graphics::detail {
    template<typename _vertex_t>
    class default_renderer : public vertex_renderer<_vertex_t> {
    public:
        default_renderer(randomcat::engine::graphics::shader _shader) : m_shader(std::move(_shader)) {
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            for (shader_input input : m_shader.inputs()) {
                glVertexAttribPointer(input.position, input.count, input.type, input.normalized, input.stride, input.offset);
                glEnableVertexAttribArray(input.position);
            }
        }

        virtual std::vector<_vertex_t>& vertices() override { return m_vertices; }
        virtual std::vector<_vertex_t> const& vertices() const override { return m_vertices; }

        void makeActive() const {
            m_shader.makeActive();
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

        void render() const {
            makeActive();

            glBufferData(GL_ARRAY_BUFFER, vertices().size() * sizeof(_vertex_t), vertices().data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, vertices().size());
        }

    private:
        vao_id_wrapper m_vao;
        vbo_id_wrapper m_vbo;
        randomcat::engine::graphics::shader m_shader;
        std::vector<_vertex_t> m_vertices;
    };
}    // namespace randomcat::engine::graphics::detail
