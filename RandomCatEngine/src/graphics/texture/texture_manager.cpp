#include "randomcat/engine/graphics/texture/texture_manager.hpp"

#include <exception>

#include <stb/stb_image.hpp>

#include "randomcat/engine/detail/log.hpp"

namespace randomcat::engine::graphics::texture {
    texture::underlying::~underlying() noexcept { stbi_image_free(m_data); }

    texture const& texture_manager::load_texture(std::string _path) {
        auto it = m_textureMap.find(_path);
        if (it != m_textureMap.end()) return it->second;

        int width, height, burn;
        auto data = stbi_load(_path.c_str(), &width, &height, &burn, STBI_rgb_alpha);

        if (data == nullptr) { throw texture_load_error{"Unable to load texture with path: " + _path}; }

        m_textureMap.insert({_path, texture{_path, width, height, data}});

        return m_textureMap.at(_path);
    }

    texture const& texture_manager::get_texture(std::string const& _path) const {
        auto it = m_textureMap.find(_path);

        if (it == m_textureMap.end()) { throw no_such_texture_error{"No texture registered with path: " + _path}; }

        return it->second;
    }
}    // namespace randomcat::engine::graphics::texture
