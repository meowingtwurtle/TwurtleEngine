#pragma once

#include <cstdint>
#include <memory>

namespace randomcat::engine::graphics::textures {
    class texture {
    public:
        explicit texture(std::int32_t _width, std::int32_t _height, unsigned char* _data) noexcept
        : m_width{_width}, m_height{_height}, m_data{_data}, m_underlying{std::make_shared<underlying>(_data)} {}

        [[nodiscard]] auto width() const noexcept { return m_width; }
        [[nodiscard]] auto height() const noexcept { return m_height; }
        [[nodiscard]] unsigned char const* data() const noexcept { return m_data; }

    private:
        struct underlying {
            unsigned char* m_data;
            underlying(unsigned char* _data) noexcept : m_data(_data) {}
            ~underlying() noexcept;
        };

        std::int32_t m_width;
        std::int32_t m_height;
        unsigned char* m_data;
        std::shared_ptr<underlying> m_underlying;
    };
}    // namespace randomcat::engine::graphics::textures
