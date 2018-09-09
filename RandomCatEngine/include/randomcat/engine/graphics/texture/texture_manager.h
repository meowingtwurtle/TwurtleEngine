#pragma once

#include <memory>
#include <string>
#include <unordered_map>

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

    class texture {
    public:
        texture();
        texture(std::string _name, int _width, int _height, unsigned char* _data);

        std::string const& name() const { return m_name; }
        int width() const { return m_width; }
        int height() const { return m_height; }
        unsigned char const* data() const { return m_underlying->m_data; }

    private:
        struct underlying {
            unsigned char* m_data;
            underlying(unsigned char* _data) : m_data(_data) {}
            ~underlying();
        };

        std::string m_name;
        int m_width;
        int m_height;
        std::shared_ptr<underlying> m_underlying;
    };

    class texture_manager {
    public:
        texture const& loadTexture(std::string const& _path);
        texture const& getTexture(std::string const& _path) const;

        using texture_map_iterator = std::unordered_map<std::string, texture>::const_iterator;

        texture_map_iterator begin() const { return m_textureMap.begin(); }
        texture_map_iterator end() const { return m_textureMap.end(); }

    private:
        std::unordered_map<std::string, texture> m_textureMap;
    };
}    // namespace randomcat::engine::graphics::texture
