#pragma once

#include <array>

#include <glm/glm.hpp>

#include "randomcat/engine/graphics/detail/default_vertex.hpp"

namespace randomcat::engine::graphics {
    namespace object_detail {
        template<typename T>
        static constexpr auto vertex_extractor_f = [](auto const& x) noexcept(noexcept(x.vertices())) -> decltype(auto) {
            static_assert(std::is_same_v<decltype(x), T const&>);
            return x.vertices();
        };

        template<typename T>
        static constexpr auto component_extractor_f = [](auto const& x) noexcept(noexcept(x.components())) -> decltype(auto) {
            static_assert(std::is_same_v<decltype(x), T const&>);
            return x.components();
        };
    }    // namespace object_detail

    class render_triangle {
    public:
        using vertex = default_vertex;

        render_triangle(vertex _posA, vertex _posB, vertex _posC) noexcept
        : posA(std::move(_posA)), posB(std::move(_posB)), posC(std::move(_posC)), m_vertices{posA, posB, posC} {}

        vertex posA;
        vertex posB;
        vertex posC;

        auto const& vertices() const noexcept { return m_vertices; }

        static constexpr auto sub_extractor_f = object_detail::vertex_extractor_f<render_triangle>;

    private:
        std::array<vertex, 3> m_vertices;
    };

    struct render_triangle_texture {
        using vertex = render_triangle::vertex;
        render_triangle shape;
        unsigned int texture;

        decltype(auto) vertices() const noexcept { return shape.vertices(); }

        static constexpr auto sub_extractor_f = object_detail::vertex_extractor_f<render_triangle_texture>;
    };

    class render_object_triangle {
    public:
        render_object_triangle(render_triangle_texture _triangle) noexcept : m_triangle{std::move(_triangle)} {}

        auto const& components() const noexcept { return m_triangle; }

        static constexpr auto sub_extractor_f = object_detail::component_extractor_f<render_object_triangle>;

    private:
        std::array<render_triangle_texture, 1> m_triangle;
    };

    class render_object_rect_prism {
    public:
        explicit render_object_rect_prism(glm::vec3 _center, glm::vec3 _sides, unsigned int _texture) noexcept
        : render_object_rect_prism(_center, _sides, _texture, _texture, _texture, _texture, _texture, _texture) {}
        explicit render_object_rect_prism(glm::vec3 _center,
                                          glm::vec3 _sides,
                                          unsigned int _texHX,
                                          unsigned int _texLX,
                                          unsigned int _texHY,
                                          unsigned int _texLY,
                                          unsigned int _texHZ,
                                          unsigned int _texLZ) noexcept
        : m_triangles(genTriangles(_center, _sides, _texHX, _texLX, _texHY, _texLY, _texHZ, _texLZ)), m_center(_center) {}

        std::array<render_triangle_texture, 12> genTriangles(glm::vec3 _center,
                                                             glm::vec3 _sides,
                                                             unsigned int _texHX,
                                                             unsigned int _texLX,
                                                             unsigned int _texHY,
                                                             unsigned int _texLY,
                                                             unsigned int _texHZ,
                                                             unsigned int _texLZ) const {
            auto const makeTriangle = [&](glm::vec3 posA, glm::vec3 posB, glm::vec3 posC, unsigned int tex, auto const& toTexCoord) {
                return render_triangle_texture{render_triangle{default_vertex{_center + (posA * _sides), toTexCoord(posA), tex},
                                                               default_vertex{_center + (posB * _sides), toTexCoord(posB), tex},
                                                               {_center + (posC * _sides), toTexCoord(posC), tex}},
                                               tex};
            };

            auto const dropHX = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.y - 0.5, pos.z - 0.5} * 1.0f; };
            auto const dropHY = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x - 0.5, pos.z - 0.5} * 1.0f; };
            auto const dropHZ = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x - 0.5, pos.y - 0.5} * 1.0f; };

            auto const dropLX = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.y + 0.5, pos.z + 0.5} * 1.0f; };
            auto const dropLY = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x + 0.5, pos.z + 0.5} * 1.0f; };
            auto const dropLZ = [](glm::vec3 pos) -> glm::vec2 { return glm::vec2{pos.x + 0.5, pos.y + 0.5} * 1.0f; };

            auto tri0 = makeTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, _texLZ, dropLZ);
            auto tri1 = makeTriangle(glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLZ, dropLZ);

            auto tri2 = makeTriangle(glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHZ, dropHZ);
            auto tri3 = makeTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, _texHZ, dropHZ);

            auto tri4 = makeTriangle(glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLX, dropLX);
            auto tri5 = makeTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, _texLX, dropLX);

            auto tri6 = makeTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, _texHX, dropHX);

            auto tri7 = makeTriangle(glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHX, dropHX);

            auto tri8 = makeTriangle(glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.5f, -0.5f, 0.5f}, _texLY, dropLY);
            auto tri9 = makeTriangle(glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{-0.5f, -0.5f, -0.5f}, _texLY, dropLY);

            auto tri10 = makeTriangle(glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, _texHY, dropHY);
            auto tri11 = makeTriangle(glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{-0.5f, 0.5f, -0.5f}, _texHY, dropHY);

            return std::array<render_triangle_texture, 12>{tri0, tri1, tri2, tri3, tri4, tri5, tri6, tri7, tri8, tri9, tri10, tri11};
        }

        auto const& components() const noexcept { return m_triangles; }
        auto center() const noexcept { return m_center; }

        static constexpr auto sub_extractor_f = object_detail::component_extractor_f<render_object_rect_prism>;

    private:
        std::array<render_triangle_texture, 12> m_triangles;
        glm::vec3 m_center;
    };

    class render_object_cube : public render_object_rect_prism {
    public:
        explicit render_object_cube(glm::vec3 _center, float _side, unsigned int _texture) noexcept
        : render_object_cube(std::move(_center), std::move(_side), _texture, _texture, _texture, _texture, _texture, _texture) {}

        explicit render_object_cube(glm::vec3 _center,
                                    float _side,
                                    unsigned int _texHX,
                                    unsigned int _texLX,
                                    unsigned int _texHY,
                                    unsigned int _texLY,
                                    unsigned int _texHZ,
                                    unsigned int _texLZ) noexcept
        : render_object_rect_prism(std::move(_center),
                                   glm::vec3{_side, _side, _side},
                                   std::move(_texHX),
                                   std::move(_texLX),
                                   std::move(_texHY),
                                   std::move(_texLY),
                                   std::move(_texHZ),
                                   std::move(_texLZ)) {}

        static constexpr auto sub_extractor_f = object_detail::component_extractor_f<render_object_cube>;
    };
}    // namespace randomcat::engine::graphics
