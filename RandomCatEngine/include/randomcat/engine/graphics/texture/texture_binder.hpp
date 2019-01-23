#pragma once

#include <GL/glew.h>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/texture_raii.hpp"
#include "randomcat/engine/graphics/texture/texture_manager.hpp"

namespace randomcat::engine::graphics::texture {
    struct texture_array {
        gl_raii_detail::unique_texture_id id;
        int width;
        int height;
        int layers;
    };

    texture_array gen_texture_array(int _width, int _height, int _layers) noexcept {
        RC_GL_ERROR_GUARD("generating texture array");

        gl_raii_detail::unique_texture_id id;
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _width, _height, _layers);

        return {std::move(id), _width, _height, _layers};
    }

    void bind_texture_array_layer(texture_array const& _array, int _layer, texture const& _texture) noexcept {
        RC_GL_ERROR_GUARD("binding texture");

        glBindTexture(GL_TEXTURE_2D_ARRAY, _array.id);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, _layer, _texture.width(), _texture.height(), 1, GL_RGBA, GL_UNSIGNED_BYTE, _texture.data());
    }
}    // namespace randomcat::engine::graphics::texture
