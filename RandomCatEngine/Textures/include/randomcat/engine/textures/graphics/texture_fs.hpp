#pragma once

#include <randomcat/util/require_filesystem.hpp>

#include "randomcat/engine/textures/graphics/texture.hpp"
#include "randomcat/engine/textures/graphics/texture_manager.hpp"

namespace randomcat::engine::graphics::textures {
    namespace texture_detail {
        struct texture_load_error_tag {};
    }    // namespace texture_detail
    using texture_load_error = util_detail::tag_exception<texture_detail::texture_load_error_tag>;

    texture load_texture_file(fs::path const& _path) noexcept(!"Throws on error");
    texture const& load_texture_file(texture_manager& _textureManager, fs::path const& _path) noexcept(!"Throws on error");
}    // namespace randomcat::engine::graphics::textures
