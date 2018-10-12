#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/graphics/detail/default_vertex.h>
#include <randomcat/engine/graphics/shader.h>

namespace randomcat::engine::graphics {
    namespace detail {
        template<typename T>
        static constexpr auto vertex_extractor_f = [](auto const& x) -> decltype(auto) {
            static_assert(std::is_same_v<decltype(x), T const&>);
            return x.vertices();
        };

        template<typename T>
        static constexpr auto component_extractor_f = [](auto const& x) -> decltype(auto) {
            static_assert(std::is_same_v<decltype(x), T const&>);
            return x.components();
        };
    }    // namespace detail

    class render_triangle {
    public:
        using vertex = detail::default_vertex;

        render_triangle(vertex _posA, vertex _posB, vertex _posC)
        : posA(std::move(_posA)), posB(std::move(_posB)), posC(std::move(_posC)), m_vertices{posA, posB, posC} {}

        vertex posA;
        vertex posB;
        vertex posC;

        std::vector<vertex> const& vertices() const { return m_vertices; }

    private:
        std::vector<vertex> m_vertices;

        static constexpr auto sub_extractor_f = detail::vertex_extractor_f<render_triangle>;
    };

    struct render_triangle_texture {
        using vertex = render_triangle::vertex;
        render_triangle shape;
        unsigned int texture;

        decltype(auto) vertices() const { return shape.vertices(); }

        static constexpr auto sub_extractor_f = detail::vertex_extractor_f<render_triangle_texture>;
    };

    class render_object_triangle {
    public:
        render_object_triangle(render_triangle_texture _triangle, graphics::shader _shader)
        : m_triangle{std::move(_triangle)}, m_shader(std::move(_shader)) {}

        std::vector<render_triangle_texture> const& components() const { return m_triangle; }
        graphics::shader const& shader() const { return m_shader; }

        static constexpr auto sub_extractor_f = detail::component_extractor_f<render_object_triangle>;

    private:
        std::vector<render_triangle_texture> m_triangle;
        graphics::shader m_shader;
    };

    class render_object_rect_prism {
    public:
        render_object_rect_prism(glm::vec3 _center, glm::vec3 _sides, unsigned int _texture, graphics::shader _shader)
        : render_object_rect_prism(_center, _sides, _texture, _texture, _texture, _texture, _texture, _texture, std::move(_shader)) {}
        render_object_rect_prism(glm::vec3 _center,
                                 glm::vec3 _sides,
                                 unsigned int _texHX,
                                 unsigned int _texLX,
                                 unsigned int _texHY,
                                 unsigned int _texLY,
                                 unsigned int _texHZ,
                                 unsigned int _texLZ,
                                 graphics::shader _shader)
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

        std::vector<render_triangle_texture> const& components() const { return m_triangles; }
        graphics::shader const& shader() const { return m_shader; }

        static constexpr auto sub_extractor_f = detail::component_extractor_f<render_object_rect_prism>;

    private:
        std::vector<render_triangle_texture> m_triangles;
        graphics::shader m_shader;
    };

    class render_object_cube : public render_object_rect_prism {
    public:
        render_object_cube(glm::vec3 _center, float _side, unsigned int _texture, graphics::shader _shader)
        : render_object_cube(std::move(_center), std::move(_side), _texture, _texture, _texture, _texture, _texture, _texture, std::move(_shader)) {}

        render_object_cube(glm::vec3 _center,
                           float _side,
                           unsigned int _texHX,
                           unsigned int _texLX,
                           unsigned int _texHY,
                           unsigned int _texLY,
                           unsigned int _texHZ,
                           unsigned int _texLZ,
                           graphics::shader _shader)
        : render_object_rect_prism(std::move(_center),
                                   glm::vec3{_side, _side, _side},
                                   std::move(_texHX),
                                   std::move(_texLX),
                                   std::move(_texHY),
                                   std::move(_texLY),
                                   std::move(_texHZ),
                                   std::move(_texLZ),
                                   std::move(_shader)) {}

        static constexpr auto sub_extractor_f = detail::component_extractor_f<render_object_cube>;
    };
}    // namespace randomcat::engine::graphics
