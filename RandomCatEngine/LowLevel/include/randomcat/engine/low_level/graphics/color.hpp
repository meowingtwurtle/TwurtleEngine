#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace randomcat::engine::graphics {
    struct color_rgba {
        using value_type = float;

        static auto constexpr opaque_alpha = 1.0f;
        static auto constexpr transparent_alpha = 0.0f;

        value_type r;
        value_type g;
        value_type b;
        value_type a;

        auto as_glm() const noexcept { return glm::vec4{r, g, b, a}; }
    };

    struct color_rgb {
        using value_type = float;

        value_type r;
        value_type g;
        value_type b;

        constexpr operator color_rgba() const noexcept { return color_rgba{r, g, b, color_rgba::opaque_alpha}; }

        auto as_glm() const noexcept { return glm::vec3{r, g, b}; }
    };

    inline auto as_glm(color_rgba _color) noexcept { return _color.as_glm(); }

    inline auto as_glm(color_rgb _color) noexcept { return _color.as_glm(); }
}    // namespace randomcat::engine::graphics
