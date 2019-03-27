#pragma once

#include <array>

#include <glm/glm.hpp>

#include "randomcat/engine/detail/impl_only_access.hpp"
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
    [[nodiscard]] constexpr decltype(auto) render_object_sub_parts(Object const& _obj) noexcept(noexcept(object_detail::object_sub_parts<Object>(_obj))) {
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

    namespace render_object_detail {
        template<typename Vertex>
        class basic_triangle {
        public:
            using vertex = Vertex;

            basic_triangle(vertex _posA, vertex _posB, vertex _posC) noexcept
            : m_posA(std::move(_posA)), m_posB(std::move(_posB)), m_posC(std::move(_posC)) {}

            auto const& operator[](std::ptrdiff_t _index) const noexcept { return std::addressof(m_posA)[_index]; }

        private:
            vertex m_posA;
            vertex m_posB;
            vertex m_posC;
        };

        template<typename Vertex>
        class basic_quad {
        public:
            using vertex = Vertex;

            basic_quad(vertex _posA, vertex _posB, vertex _posC, vertex _posD) noexcept
            : m_posA(std::move(_posA)), m_posB(std::move(_posB)), m_posC(std::move(_posC)), m_posD(std::move(_posD)) {}

            auto const& operator[](std::ptrdiff_t _index) const noexcept { return std::addressof(m_posA)[_index]; }

        private:
            vertex m_posA;
            vertex m_posB;
            vertex m_posC;
            vertex m_posD;
        };
    }    // namespace render_object_detail

    using location_triangle = render_object_detail::basic_triangle<default_vertex::location>;
    using texture_triangle = render_object_detail::basic_triangle<default_vertex::texture>;
    using location_quad = render_object_detail::basic_quad<default_vertex::location>;
    using texture_quad = render_object_detail::basic_quad<default_vertex::texture>;

    class render_object_triangle {
    public:
        using vertex = default_vertex;

        render_object_triangle(location_triangle _locations, texture_triangle _texture) noexcept
        : m_vertices{vertex{_locations[0], _texture[0]}, vertex{_locations[1], _texture[1]}, vertex{_locations[2], _texture[2]}} {
            set_normal(impl_call, -glm::normalize(glm::cross(_locations[1].value - _locations[0].value, _locations[2].value - _locations[1].value)));
        }

        auto const& vertices() const noexcept { return m_vertices; }

        void set_normal(impl_call_only, glm::vec3 _normal) noexcept {
            m_normal = _normal;
            std::for_each(begin(m_vertices), end(m_vertices), [&](auto& v) { v.normal = _normal; });
        }

        RC_SUB_PARTS(vertices);

    private:
        std::array<vertex, 3> m_vertices;
        glm::vec3 m_normal;
    };

    class render_object_rectangle {
    public:
        render_object_rectangle(location_quad _locations, texture_quad _texture) noexcept
        : m_triangles{render_object_triangle{location_triangle{_locations[0], _locations[1], _locations[2]},
                                             texture_triangle{_texture[0], _texture[1], _texture[2]}},
                      render_object_triangle{location_triangle{_locations[0], _locations[2], _locations[3]},
                                             texture_triangle{_texture[0], _texture[2], _texture[3]}}} {}

        auto const& triangles() const noexcept { return m_triangles; }

        RC_SUB_PARTS(triangles);

    private:
        std::array<render_object_triangle, 2> m_triangles;
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
        : m_rectangles(gen_triangles(_center, _sides, _texHX, _texLX, _texHY, _texLY, _texHZ, _texLZ)), m_center(_center) {}

        auto const& sides() const noexcept { return m_rectangles; }
        auto center() const noexcept { return m_center; }

        RC_SUB_PARTS(sides);

    private:
        std::array<render_object_rectangle, 6> gen_triangles(glm::vec3 _center,
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

            auto location = [](glm::vec3 posA, glm::vec3 posB, glm::vec3 posC, glm::vec3 posD) noexcept {
                return location_quad{{posA}, {posB}, {posC}, {posD}};
            };

            auto texture = [&](texture_array_index layer) noexcept {
                return texture_quad{{{0, 0}, layer}, {{1, 0}, layer}, {{1, 1}, layer}, {{0, 1}, layer}};
            };

            auto rectHZ = render_object_rectangle{location(pointLXHYHZ, pointHXHYHZ, pointHXLYHZ, pointLXLYHZ), texture(_texHZ)};
            auto rectLZ = render_object_rectangle{location(pointHXHYLZ, pointLXHYLZ, pointLXLYLZ, pointHXLYLZ), texture(_texLZ)};

            auto rectHX = render_object_rectangle{location(pointHXHYHZ, pointHXHYLZ, pointHXLYLZ, pointHXLYHZ), texture(_texHX)};
            auto rectLX = render_object_rectangle{location(pointLXHYLZ, pointLXHYHZ, pointLXLYHZ, pointLXLYLZ), texture(_texLX)};

            auto rectHY = render_object_rectangle{location(pointHXHYHZ, pointLXHYHZ, pointLXHYLZ, pointHXHYLZ), texture(_texHY)};
            auto rectLY = render_object_rectangle{location(pointHXLYLZ, pointLXLYLZ, pointLXLYHZ, pointHXLYHZ), texture(_texLY)};

            return {rectHZ, rectLZ, rectHY, rectLY, rectLX, rectHX};
        }

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

    class render_object_regular_polygon {
    public:
        render_object_regular_polygon(int sides, glm::vec3 center, float radius, texture_array_index _texture)
        : m_sideCount(std::move(sides)) {
            for (int i = 0; i < sides; ++i) {
                auto theta0 = units::degrees(360.0f * i / sides);
                auto theta1 = units::degrees(360.0f * (i + 1) / sides);
                auto point0 = center + radius * glm::vec3(cos(theta0), sin(theta0), 0);
                auto point1 = center + radius * glm::vec3(cos(theta1), sin(theta1), 0);

                auto texturePoint = [&](auto const& theta) {
                    return default_vertex::texture{{(cos(theta) + 1.f) / 2.f, (1 - sin(theta)) / 2.f}, _texture};
                };

                m_triangles.push_back(render_object_triangle(location_triangle{{center}, {point0}, {point1}},
                                                             texture_triangle{{{0.5, 0.5}, _texture}, texturePoint(theta0), texturePoint(theta1)}));
            }
        }

        auto const& triangles() const noexcept { return m_triangles; }
        auto side_count() const noexcept { return m_sideCount; }

        RC_SUB_PARTS(triangles);

    private:
        std::vector<render_object_triangle> m_triangles;
        int m_sideCount;
    };

#undef RC_SUB_PARTS
}    // namespace randomcat::engine::graphics
