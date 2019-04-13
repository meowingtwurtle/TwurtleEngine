#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "randomcat/engine/low_level/detail/tag_exception.hpp"
#include "randomcat/engine/textures/graphics/texture.hpp"

namespace randomcat::engine::graphics::textures {
    namespace texture_detail {
        struct no_such_texture_error_tag {};
    }    // namespace texture_detail
    using no_such_texture_error = util_detail::tag_exception<texture_detail::no_such_texture_error_tag>;

    namespace texture_detail {
        struct texture_duplicate_path_tag {};
    }    // namespace texture_detail
    using texture_duplicate_path_error = util_detail::tag_exception<texture_detail::texture_duplicate_path_tag>;

    class texture_manager {
    public:
        [[nodiscard]] texture const& get_texture(std::string_view _path) const noexcept(!"Throws on error");
        [[nodiscard]] bool has_texture(std::string_view _path) const noexcept;

        // Returns whether or not a texture was removed
        [[nodiscard]] bool remove_texture(std::string_view _path) noexcept;

        void alias_texture(std::string _newName, std::string_view _oldName) noexcept(!"Throws on error");

        // Copies the texture into the map, returns a reference to the new texture
        texture const& add_texture(std::string _newName, texture _texture) noexcept(!"Throws on error");

    private:
        using map_t = std::unordered_map<std::string, texture>;
        using map_iter_t = typename map_t::iterator;
        using map_const_iter_t = typename map_t::const_iterator;

        map_iter_t texture_iter(std::string_view _name) noexcept;
        map_const_iter_t texture_iter(std::string_view _name) const noexcept;
        map_t m_textureMap;
    };

}    // namespace randomcat::engine::graphics::textures
