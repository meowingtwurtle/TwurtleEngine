#pragma once

#include "randomcat/engine/low_level/graphics/color.hpp"
#include "randomcat/engine/textures/graphics/texture.hpp"

namespace randomcat::engine::graphics::textures {
    texture color_texture(texture::dimension_t _width, texture::dimension_t _height, color_rgba _color) {
        auto const channels = texture::channels;
        auto const size = _width * _height * channels;
        auto data = std::make_unique<unsigned char[]>(size);

        using T = unsigned char;

        for (auto i = 0; i < (_width * _height); ++i) {
            data[i * channels + 0] = T(_color.r * 255);
            data[i * channels + 1] = T(_color.g * 255);
            data[i * channels + 2] = T(_color.b * 255);
            data[i * channels + 3] = T(_color.a * 255);
        }

        return texture(impl_call, _width, _height, texture::take_ownership, std::move(data));
    }
}    // namespace randomcat::engine::graphics::textures
