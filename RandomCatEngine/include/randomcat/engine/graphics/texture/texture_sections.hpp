#pragma once

#include <glm/vec2.hpp>

#include "randomcat/engine/graphics/texture_array_index.hpp"

namespace randomcat::engine::graphics::textures {
    struct texture_triangle {
    public:
        using layer_type = texture_array_index;
        using location_type = glm::vec2;
        using coordinate_type = location_type::value_type;

        /* implicit */ texture_triangle(layer_type _layer, location_type _posA, location_type _posB, location_type _posC) noexcept
        : m_layer(_layer), m_posA{std::move(_posA)}, m_posB{std::move(_posB)}, m_posC{std::move(_posC)} {}

        auto const& operator[](std::ptrdiff_t _index) const noexcept { return std::addressof(m_posA)[_index]; }

        auto layer() const noexcept { return m_layer; }

    private:
        layer_type m_layer;
        location_type m_posA;
        location_type m_posB;
        location_type m_posC;
    };

    class texture_quad {
    public:
        using layer_type = texture_array_index;
        using location_type = glm::vec2;
        using coordinate_type = location_type::value_type;

        /* implicit */ texture_quad(layer_type _layer, location_type _posA, location_type _posB, location_type _posC, location_type _posD) noexcept
        : m_layer(_layer), m_posA{std::move(_posA)}, m_posB{std::move(_posB)}, m_posC{std::move(_posC)}, m_posD{std::move(_posD)} {}

        auto const& operator[](std::ptrdiff_t _index) const noexcept { return std::addressof(m_posA)[_index]; }

        auto layer() const noexcept { return m_layer; }

    private:
        layer_type m_layer;
        location_type m_posA;
        location_type m_posB;
        location_type m_posC;
        location_type m_posD;
    };

    class texture_rectangle : public texture_quad {
    public:
        using dimensions_type = glm::vec2;
        using dimension_type = dimensions_type::value_type;

        struct from_corners_t {};
        static auto constexpr from_corners = from_corners_t{};

        explicit texture_rectangle(layer_type _layer, from_corners_t, location_type _first, location_type _second) noexcept
        : texture_quad{_layer, _first, {_second.x, _first.y}, _second, {_first.x, _second.y}} {}

        struct from_corner_and_dimensions_t {};
        static auto constexpr from_corner_and_dimensions = from_corner_and_dimensions_t{};
        explicit texture_rectangle(layer_type _layer, from_corner_and_dimensions_t, location_type _corner, coordinate_type _xDim, coordinate_type _yDim) noexcept
        : texture_rectangle{_layer, from_corners, _corner, {_corner.x + _xDim, _corner.y + _yDim}} {}

        explicit texture_rectangle(layer_type _layer, from_corner_and_dimensions_t, location_type _corner, dimensions_type _dimensions)
        : texture_rectangle{_layer, from_corner_and_dimensions, _corner, _dimensions.x, _dimensions.y} {}

        decltype(auto) top_left() const noexcept { return (*this)[0]; }

        decltype(auto) top_right() const noexcept { return (*this)[1]; }

        decltype(auto) bottom_right() const noexcept { return (*this)[2]; }

        decltype(auto) bottom_left() const noexcept { return (*this)[3]; }

        location_type center() const noexcept { return (top_left() + bottom_right()) / 2.0f; }

        dimensions_type dimensions() const noexcept { return (bottom_right() - top_left()); }

        dimension_type x_dimension() const noexcept { return dimensions().x; }

        dimension_type y_dimension() const noexcept { return dimensions().y; }
    };
}    // namespace randomcat::engine::graphics::textures
