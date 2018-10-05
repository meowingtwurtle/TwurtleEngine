#pragma once

#include <type_traits>

#include <randomcat/engine/graphics/detail/default_renderer.h>
#include <randomcat/engine/graphics/detail/default_vertex.h>
#include <randomcat/engine/graphics/object.h>
#include <randomcat/engine/graphics/renderer.h>

namespace randomcat::engine::graphics {
    template<typename _object_t, typename _renderer_t = detail::default_renderer<typename _object_t::component::vertex>>
    class default_object_renderer : public object_renderer<_object_t> {
    public:
        using object = _object_t;
        using component = typename object::component;
        using vertex = typename component::vertex;
        using renderer = _renderer_t;

        virtual void render() const override {
            for (auto const& object : objects()) {
                auto const components = object.components();

                std::vector<vertex> vertices;

                for (auto const& component : components) {
                    auto const subVertices = component.vertices();
                    vertices.insert(std::end(vertices), std::begin(subVertices), std::end(subVertices));
                }

                auto vertexRenderer = renderer{object.shader()};
                vertexRenderer.vertices() = std::move(vertices);
                vertexRenderer.render();
            }
        }

        virtual std::vector<object>& objects() override { return m_objects; }
        virtual std::vector<object> const& objects() const override { return m_objects; }

    private:
        std::vector<object> m_objects;
    };
}    // namespace randomcat::engine::graphics
