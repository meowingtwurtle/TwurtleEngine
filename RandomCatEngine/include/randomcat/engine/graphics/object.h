#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/graphics/detail/default_vertex.h>
#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    struct render_triangle {
        using vertex = detail::default_vertex;
        vertex posA;
        vertex posB;
        vertex posC;

        std::vector<vertex> vertices() const { return {posA, posB, posC}; }
    };

    struct render_triangle_texture {
        using vertex = render_triangle::vertex;
        render_triangle shape;
        unsigned int texture;

        std::vector<vertex> vertices() const { return shape.vertices(); }
    };

    template<typename _component_t>
    class render_object {
    public:
        using component = _component_t;

        virtual ~render_object() = default;
        virtual std::vector<component> const& components() const = 0;
        virtual shader const& getShader() const = 0;
    };

    class render_object_triangle : public render_object<render_triangle_texture> {
    public:
        render_object_triangle(render_triangle_texture _triangle, class shader _shader)
        : m_triangle{std::move(_triangle)}, m_shader(std::move(_shader)) {}

        virtual std::vector<render_triangle_texture> const& components() const override { return m_triangle; }
        virtual class shader const& getShader() const override { return m_shader; }

    private:
        std::vector<render_triangle_texture> m_triangle;
        class shader m_shader;
    };

    class render_object_rect_prism : public render_object<render_triangle_texture> {
    public:
        render_object_rect_prism(glm::vec3 _center, glm::vec3 _sides, unsigned int _texture, class shader _shader)
        : render_object_rect_prism(_center, _sides, _texture, _texture, _texture, _texture, _texture, _texture, std::move(_shader)) {}
        render_object_rect_prism(glm::vec3 _center,
                                 glm::vec3 _sides,
                                 unsigned int _texHX,
                                 unsigned int _texLX,
                                 unsigned int _texHY,
                                 unsigned int _texLY,
                                 unsigned int _texHZ,
                                 unsigned int _texLZ,
                                 class shader _shader)
        : m_shader(std::move(_shader)) {
            m_triangles.reserve(12);

            auto const addTriangle = [&, this](glm::vec3 posA, glm::vec3 posB, glm::vec3 posC, unsigned int tex, auto const& toTexCoord) {
                m_triangles.push_back(
                    render_triangle_texture{render_triangle{detail::default_vertex{_center + (posA * _sides), toTexCoord(posA), tex},
                                                            detail::default_vertex{_center + (posB * _sides), toTexCoord(posB), tex},
                                                            {_center + (posC * _sides), toTexCoord(posC), tex}},
                                            tex});
            };

            auto const dropHX = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.y - 0.5, pos.z - 0.5} * 1.0f; };
            auto const dropHY = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x - 0.5, pos.z - 0.5} * 1.0f; };
            auto const dropHZ = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x - 0.5, pos.y - 0.5} * 1.0f; };

            auto const dropLX = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.y + 0.5, pos.z + 0.5} * 1.0f; };
            auto const dropLY = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x + 0.5, pos.z + 0.5} * 1.0f; };
            auto const dropLZ = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x + 0.5, pos.y + 0.5} * 1.0f; };

            addTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, _texLZ, dropLZ);
            addTriangle(glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLZ, dropLZ);

            addTriangle(glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHZ, dropHZ);
            addTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, _texHZ, dropHZ);

            addTriangle(glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLX, dropLX);
            addTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, _texLX, dropLX);

            addTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, _texHX, dropHX);

            addTriangle(glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHX, dropHX);

            addTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, _texLY, dropLY);
            addTriangle(glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLY, dropLY);

            addTriangle(glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHY, dropHY);
            addTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, _texHY, dropHY);
        }

        virtual std::vector<render_triangle_texture> const& components() const override { return m_triangles; }
        virtual class shader const& getShader() const override { return m_shader; }

    private:
        std::vector<render_triangle_texture> m_triangles;
        class shader m_shader;
    };

    class render_object_cube : public render_object_rect_prism {
    public:
        render_object_cube(glm::vec3 _center, float _side, unsigned int _texture, shader _shader)
        : render_object_cube(std::move(_center), std::move(_side), _texture, _texture, _texture, _texture, _texture, _texture, std::move(_shader)) {}

        render_object_cube(glm::vec3 _center,
                           float _side,
                           unsigned int _texHX,
                           unsigned int _texLX,
                           unsigned int _texHY,
                           unsigned int _texLY,
                           unsigned int _texHZ,
                           unsigned int _texLZ,
                           class shader _shader)
        : render_object_rect_prism(std::move(_center),
                                   glm::vec3{_side, _side, _side},
                                   std::move(_texHX),
                                   std::move(_texLX),
                                   std::move(_texHY),
                                   std::move(_texLY),
                                   std::move(_texHZ),
                                   std::move(_texLZ),
                                   std::move(_shader)) {}
    };
}    // namespace randomcat::engine::graphics
