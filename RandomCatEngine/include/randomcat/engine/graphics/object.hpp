#pragma once

#include <array>

#include <glm/glm.hpp>

#include "randomcat/engine/detail/templates.hpp"
#include "randomcat/engine/graphics/detail/default_vertex.hpp"

namespace randomcat::engine::graphics {
    namespace object_detail {
        // Implementation, do not specialize
        template<typename Object, typename = void>
        struct object_has_static_sub_parts_func_s : std::false_type {};

        template<typename Object>
        struct object_has_static_sub_parts_func_s<Object, std::void_t<decltype(Object::sub_parts(std::declval<Object const&>()))>> :
        std::true_type {};

        template<typename Object>
        static auto constexpr object_has_static_sub_parts_func = object_has_static_sub_parts_func_s<Object>::value;

        template<typename Object, typename = void>
        struct object_has_member_sub_parts_func_s : std::false_type {};

        template<typename Object>
        struct object_has_member_sub_parts_func_s<Object, std::void_t<decltype(std::declval<Object const&>().sub_parts())>> : std::true_type {};

        template<typename Object>
        static auto constexpr object_has_member_sub_parts_func = object_has_member_sub_parts_func_s<Object>::value;

        // This may be specialized by users and will receive full library support
        // By default, will use a static function called "sub_parts" taking an object of
        // the render object type by const& or a const member function named sub_parts
        // taking no arguments If specializing: exposes a variable called "value" that
        // returns the sub_parts when called as `std::invoke(value, obj)` Where obj is a
        // const reference to the Object type
        template<typename Object, typename = void>
        struct object_sub_parts_s {
            static_assert(util_detail::invalid<Object>,
                          "Unrecognized render object type, you may specialize this "
                          "struct if needed.");
        };

        template<typename Object>
        struct object_sub_parts_s<Object, std::enable_if_t<object_has_static_sub_parts_func<Object>>> {
            static auto constexpr value = Object::sub_parts;
        };

        template<typename Object>
        struct object_sub_parts_s<Object, std::enable_if_t<object_has_member_sub_parts_func<Object>>> {
            static auto constexpr value = [](Object const& obj) noexcept(noexcept(obj.sub_parts())) -> decltype(auto) {
                return obj.sub_parts();
            };
        };

        template<typename Object>
        static auto constexpr object_sub_parts = object_sub_parts_s<Object>::value;
    }    // namespace object_detail

    template<typename Object>
    constexpr decltype(auto) render_object_sub_parts(Object const& _obj) noexcept(noexcept(object_detail::object_sub_parts<Object>(_obj))) {
        return std::invoke(object_detail::object_sub_parts<Object>, _obj);
    }

    template<typename Target, typename InputIt, typename OutputIt>
    constexpr OutputIt decompose_render_object_to(InputIt _begin, InputIt _end, OutputIt _output) noexcept {
        using InputType = typename std::iterator_traits<InputIt>::value_type;

        if constexpr (std::is_same_v<InputType, Target>) {
            return std::copy(_begin, _end, _output);
        } else {
            std::for_each(_begin, _end, [&](auto const& x) {
                decltype(auto) sub = render_object_sub_parts(x);
                _output = decompose_render_object_to<Target>(begin(sub), end(sub), _output);
            });
            return _output;
        }
    }

    template<typename Target, typename Object, typename OutputIt>
    constexpr OutputIt decompose_render_object_to(Object const& _obj, OutputIt _output) noexcept {
        return decompose_render_object_to<Target>(std::addressof(_obj), std::addressof(_obj) + 1, std::move(_output));
    }

// Macro because this really needs it and really shortens the code
// Must be a template to delay instantiation until the render_object type is a
// complete type
#define RC_SUB_PARTS(name)                                                                                                                 \
    template<bool Impl = true>                                                                                                             \
    constexpr decltype(auto) sub_parts() const noexcept(noexcept(std::enable_if_t<Impl, decltype(*this)>(*this).name())) {                 \
        return std::enable_if_t<Impl, decltype(*this)>(*this).name();                                                                      \
    }

    class render_triangle {
    public:
        using vertex = default_vertex;

        render_triangle(vertex _posA, vertex _posB, vertex _posC) noexcept
        : posA(std::move(_posA)), posB(std::move(_posB)), posC(std::move(_posC)), m_vertices{posA, posB, posC} {}

        vertex posA;
        vertex posB;
        vertex posC;

        auto const& vertices() const noexcept { return m_vertices; }

        RC_SUB_PARTS(vertices);

    private:
        std::array<vertex, 3> m_vertices;
    };

    struct render_triangle_texture {
        using vertex = render_triangle::vertex;
        render_triangle shape;
        texture_array_index texture;

        decltype(auto) vertices() const noexcept { return shape.vertices(); }

        RC_SUB_PARTS(vertices);
    };

    class render_object_triangle {
    public:
        render_object_triangle(render_triangle_texture _triangle) noexcept : m_triangle{std::move(_triangle)} {}

        decltype(auto) vertices() const noexcept { return m_triangle.vertices(); }
        auto const& triangle() const noexcept { return m_triangle; }

        RC_SUB_PARTS(vertices);

    private:
        render_triangle_texture m_triangle;
    };

    class render_object_rectangle {
    public:
        explicit render_object_rectangle(glm::vec3 _topLeft, glm::vec3 _topRight, glm::vec3 _bottomLeft, texture_array_index _texture) noexcept
        : m_triangles(gen_triangles(std::move(_topLeft), std::move(_topRight), std::move(_bottomLeft), std::move(_texture))) {}

        auto const& triangles() const noexcept { return m_triangles; }

        RC_SUB_PARTS(triangles);

    private:
        static std::array<render_triangle_texture, 2> gen_triangles(glm::vec3 _topLeft, glm::vec3 _topRight, glm::vec3 _bottomLeft, texture_array_index _texture) noexcept {
            auto bottomRight = _bottomLeft + (_topRight - _topLeft);

            auto bottomLeftV = default_vertex{_bottomLeft, {0, 1}, _texture};
            auto bottomRightV = default_vertex{bottomRight, {1, 1}, _texture};
            auto topLeftV = default_vertex{_topLeft, {0, 0}, _texture};
            auto topRightV = default_vertex{_topRight, {1, 0}, _texture};

            return {render_triangle_texture{render_triangle{topRightV, topLeftV, bottomRightV}, _texture},
                    render_triangle_texture{render_triangle{bottomLeftV, bottomRightV, topLeftV}, _texture}};
        }

        std::array<render_triangle_texture, 2> m_triangles;
    };

    class render_object_rect_prism {
    public:
        explicit render_object_rect_prism(glm::vec3 _center, glm::vec3 _sides, texture_array_index _texture) noexcept
        : render_object_rect_prism(_center, _sides, _texture, _texture, _texture, _texture, _texture, _texture) {}
        explicit render_object_rect_prism(glm::vec3 _center,
                                          glm::vec3 _sides,
                                          texture_array_index _texHX,
                                          texture_array_index _texLX,
                                          texture_array_index _texHY,
                                          texture_array_index _texLY,
                                          texture_array_index _texHZ,
                                          texture_array_index _texLZ) noexcept
        : m_rectangles(genTriangles(_center, _sides, _texHX, _texLX, _texHY, _texLY, _texHZ, _texLZ)), m_center(_center) {}

        std::array<render_object_rectangle, 6> genTriangles(glm::vec3 _center,
                                                            glm::vec3 _sides,
                                                            texture_array_index _texHX,
                                                            texture_array_index _texLX,
                                                            texture_array_index _texHY,
                                                            texture_array_index _texLY,
                                                            texture_array_index _texHZ,
                                                            texture_array_index _texLZ) const noexcept {
            auto vecHX = glm::vec3{_sides.x / 2, 0, 0};
            auto vecHY = glm::vec3{0, _sides.y / 2, 0};
            auto vecHZ = glm::vec3{0, 0, _sides.z / 2};
            auto vecLX = -vecHX;
            auto vecLY = -vecHY;
            auto vecLZ = -vecHZ;

            // Sorry for the macro, takes L and H (low and high) and generates corners
            // of the cubes

#define RC_POINT_GEN(x, y, z) auto point##x##X##y##Y##z##Z = _center + vec##x##X + vec##y##Y + vec##z##Z;

            RC_POINT_GEN(L, L, L);
            RC_POINT_GEN(L, L, H);
            RC_POINT_GEN(L, H, L);
            RC_POINT_GEN(L, H, H);
            RC_POINT_GEN(H, L, L);
            RC_POINT_GEN(H, L, H);
            RC_POINT_GEN(H, H, L);
            RC_POINT_GEN(H, H, H);

#undef RC_POINT_GEN

            auto rectHZ = render_object_rectangle{pointLXHYHZ, pointHXHYHZ, pointLXLYHZ, _texHZ};
            auto rectLZ = render_object_rectangle{pointHXHYLZ, pointLXHYLZ, pointHXLYLZ, _texLZ};

            auto rectHX = render_object_rectangle{pointHXHYHZ, pointHXHYLZ, pointHXLYHZ, _texHX};
            auto rectLX = render_object_rectangle{pointLXHYLZ, pointLXHYHZ, pointLXLYLZ, _texLX};

            auto rectHY = render_object_rectangle{pointHXHYHZ, pointLXHYHZ, pointHXHYLZ, _texHY};
            auto rectLY = render_object_rectangle{pointHXLYLZ, pointLXLYLZ, pointHXLYHZ, _texLY};

            return {rectHZ, rectLZ, rectHY, rectLY, rectLX, rectHX};
        }

        auto const& sides() const noexcept { return m_rectangles; }
        auto center() const noexcept { return m_center; }

        RC_SUB_PARTS(sides);

    private:
        std::array<render_object_rectangle, 6> m_rectangles;
        glm::vec3 m_center;
    };

    class render_object_cube : public render_object_rect_prism {
    public:
        explicit render_object_cube(glm::vec3 _center, GLfloat _side, texture_array_index _texture) noexcept
        : render_object_cube(std::move(_center), std::move(_side), _texture, _texture, _texture, _texture, _texture, _texture) {}

        explicit render_object_cube(glm::vec3 _center,
                                    float _side,
                                    texture_array_index _texHX,
                                    texture_array_index _texLX,
                                    texture_array_index _texHY,
                                    texture_array_index _texLY,
                                    texture_array_index _texHZ,
                                    texture_array_index _texLZ) noexcept
        : render_object_rect_prism(std::move(_center),
                                   glm::vec3{_side, _side, _side},
                                   std::move(_texHX),
                                   std::move(_texLX),
                                   std::move(_texHY),
                                   std::move(_texLY),
                                   std::move(_texHZ),
                                   std::move(_texLZ)) {}
    };

#undef RC_SUB_PARTS
}    // namespace randomcat::engine::graphics
