#include <randomcat/engine/detail/log.h>
#include <randomcat/engine/graphics/texture/stb_image.h>
#include <randomcat/engine/graphics/texture/texture_manager.h>

namespace randomcat::engine::graphics::texture {
    struct texture_not_found {
        std::string path;

        std::string what() { return "No texture registered with path: " + path; }
    };

    struct texture_reregister {
        std::string path;

        std::string what() { return "Attempt to re-register texture with path: " + path; }
    };

    struct texture_load_failure {
        std::string path;

        std::string what() { return "Unable to find texture with path: " + path; }
    };

    texture::texture() : m_name(""), m_width(0), m_height(0), m_underlying(std::make_shared<underlying>(nullptr)) {}
    texture::texture(std::string _name, int _width, int _height, unsigned char* _data)
    : m_name(std::move(_name)), m_width(_width), m_height(_height), m_underlying(std::make_shared<underlying>(_data)) {}

    texture::underlying::~underlying() {
        if (m_data != nullptr) stbi_image_free(static_cast<void*>(m_data));
    }

    texture const& texture_manager::loadTexture(std::string const& _path) {
        auto it = m_textureMap.find(_path);
        if (it != m_textureMap.end()) throw texture_reregister{_path};

        int width, height, burn;
        auto data = stbi_load(_path.c_str(), &width, &height, &burn, 0);

        if (data == nullptr) {
            log::error("Unable to load texture with path: " + _path);
            throw texture_load_failure{_path};
        }

        m_textureMap.insert({_path, texture{_path, width, height, data}});

        return m_textureMap.at(_path);
    }

    texture const& texture_manager::getTexture(std::string const& _path) const {
        auto it = m_textureMap.find(_path);

        if (it == m_textureMap.end()) {
            log::error("No texture registered with path: " + _path);
            throw texture_not_found{_path};
        }

        return it->second;
    }
}    // namespace randomcat::engine::graphics::texture
