#include "randomcat/engine/graphics/texture/texture_manager.hpp"

#include <exception>

#include <gsl/gsl_util>
#include <stb/stb_image.hpp>

#include "randomcat/engine/detail/log.hpp"

namespace randomcat::engine::graphics::textures {
    texture::underlying::~underlying() noexcept { stbi_image_free(m_data); }

    texture const& texture_manager::get_texture(std::string_view _path) const {
        auto it = texture_iter(_path);
        if (it == end(m_textureMap)) { throw no_such_texture_error{"No texture registered with path: " + std::string(_path)}; }

        return it->second;
    }

    bool texture_manager::has_texture(std::string_view _path) const noexcept { return texture_iter(_path) != end(m_textureMap); }

    bool texture_manager::remove_texture(std::string_view _path) noexcept {
        auto nodeIt = texture_iter(_path);
        if (nodeIt == end(m_textureMap)) return false;

        m_textureMap.erase(nodeIt);
        return true;
    }

    texture_manager::map_iter_t texture_manager::texture_iter(std::string_view _name) noexcept {
        return std::find_if(begin(m_textureMap), end(m_textureMap), [&](auto node) { return node.first == _name; });
    }

    texture_manager::map_const_iter_t texture_manager::texture_iter(std::string_view _name) const noexcept {
        return std::find_if(begin(m_textureMap), end(m_textureMap), [&](auto node) { return node.first == _name; });
    }

    texture const& texture_manager::add_texture(std::string _newName, texture _texture) noexcept(false) {
        if (has_texture(_newName)) throw texture_duplicate_path_error("Attempted register of path that already exists: " + _newName);
        auto insertResult = m_textureMap.insert({std::move(_newName), std::move(_texture)});

        if (!insertResult.second) {
            // Old name is guaranteed to be equal to new name, so use the old name
            throw texture_duplicate_path_error{"Attempt to register texture with path that already exists: " + insertResult.first->first};
        }

        return insertResult.first->second;
    }

    void texture_manager::alias_texture(std::string _newName, std::string_view _oldName) noexcept(false) {
        add_texture(std::move(_newName), get_texture(_oldName));
    }
}    // namespace randomcat::engine::graphics::textures
