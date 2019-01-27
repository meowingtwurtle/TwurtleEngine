#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "randomcat/engine/detail/tag_exception.hpp"

namespace randomcat::engine::graphics::texture {
    class texture {
    public:
        explicit texture(std::string _name, int _width, int _height, unsigned char* _data) noexcept
        : m_name(std::move(_name)), m_width(_width), m_height(_height), m_data(_data), m_underlying(std::make_shared<underlying>(_data)) {}

        std::string const& name() const noexcept { return m_name; }
        int width() const noexcept { return m_width; }
        int height() const noexcept { return m_height; }
        unsigned char const* data() const noexcept { return m_data; }

    private:
        struct underlying {
            unsigned char* m_data;
            underlying(unsigned char* _data) noexcept : m_data(_data) {}
            ~underlying() noexcept;
        };

        std::string m_name;
        int m_width;
        int m_height;
        unsigned char* m_data;
        std::shared_ptr<underlying> m_underlying;
    };

    namespace texture_detail {
        struct texture_load_error_tag {};
    }    // namespace texture_detail
    using texture_load_error = util_detail::tag_exception<texture_detail::texture_load_error_tag>;

    namespace texture_detail {
        struct no_such_texture_error_tag {};
    }    // namespace texture_detail
    using no_such_texture_error = util_detail::tag_exception<texture_detail::no_such_texture_error_tag>;

    class texture_manager {
    public:
        texture const& load_texture(std::string const& _path);
        texture const& get_texture(std::string const& _path) const;

        using texture_map_iterator = std::unordered_map<std::string, texture>::const_iterator;

        texture_map_iterator begin() const noexcept { return m_textureMap.begin(); }
        texture_map_iterator end() const noexcept { return m_textureMap.end(); }

    private:
        std::unordered_map<std::string, texture> m_textureMap;
    };
}    // namespace randomcat::engine::graphics::texture
