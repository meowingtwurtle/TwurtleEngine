#pragma once

#include <cstdint>
#include <memory>

#include "randomcat/engine/detail/impl_only_access.hpp"

namespace randomcat::engine::graphics::textures {
    class texture {
    public:
        using private_image_value = unsigned char;
        using public_image_value = unsigned char const;

        using private_image_ptr = std::add_pointer_t<private_image_value>;
        using public_image_ptr = std::add_pointer_t<public_image_value>;

        using dimension_t = std::int32_t;

        static auto constexpr channels = 4;

        struct stbi_memory_tag {};
        static auto constexpr stbi_memory = stbi_memory_tag{};

        struct copy_memory_tag {};
        static auto constexpr copy_memory = copy_memory_tag{};

        struct take_ownership_tag {};
        static auto constexpr take_ownership = take_ownership_tag{};

        explicit texture(impl_call_only, dimension_t _width, dimension_t _height, stbi_memory_tag, private_image_ptr _data) noexcept
        : texture(_width, _height, std::make_unique<stbi_underlying>(_data)) {}

        explicit texture(impl_call_only, dimension_t _width, dimension_t _height, copy_memory_tag, private_image_ptr _data) noexcept
        : texture(_width, _height, std::make_unique<copy_memory_underlying>(_data, _width * _height * channels)) {}

        explicit texture(impl_call_only, dimension_t _width, dimension_t _height, take_ownership_tag, std::unique_ptr<private_image_value[]> _data) noexcept
        : texture(_width, _height, std::make_unique<take_ownership_underlying>(std::move(_data))) {}

        [[nodiscard]] auto width() const noexcept { return m_width; }
        [[nodiscard]] auto height() const noexcept { return m_height; }
        [[nodiscard]] public_image_ptr data(impl_call_only) const noexcept { return m_data; }

    private:
        struct underlying;

        explicit texture(std::int32_t _width, std::int32_t _height, std::unique_ptr<underlying> _underlying)
        : m_width{std::move(_width)}, m_height{std::move(_height)}, m_underlying{std::move(_underlying)}, m_data(m_underlying->data()) {}

        struct underlying {
            underlying() noexcept = default;

            underlying(underlying const&) = delete;
            underlying(underlying&&) = delete;
            underlying& operator=(underlying const&) = delete;
            underlying& operator=(underlying&&) = delete;

            virtual ~underlying() noexcept = default;
            virtual private_image_ptr data() const noexcept = 0;
        };

        struct stbi_underlying : underlying {
        public:
            stbi_underlying(private_image_ptr _stbiPtr) noexcept : m_stbiPtr(_stbiPtr) {}
            virtual ~stbi_underlying() noexcept override;

            virtual private_image_ptr data() const noexcept override { return m_stbiPtr; }

        private:
            private_image_ptr m_stbiPtr;
        };

        struct copy_memory_underlying : underlying {
        public:
            copy_memory_underlying(private_image_ptr _data, std::int32_t _length)
            : m_data(std::make_unique<private_image_value[]>(_length)) {
                std::copy(_data, _data + _length, m_data.get());
            }
            virtual ~copy_memory_underlying() noexcept override = default;

            virtual private_image_ptr data() const noexcept override { return m_data.get(); }

        private:
            std::unique_ptr<private_image_value[]> m_data;
        };

        struct take_ownership_underlying : underlying {
        public:
            take_ownership_underlying(std::unique_ptr<private_image_value[]> _data) : m_data(std::move(_data)) {}
            virtual ~take_ownership_underlying() noexcept override = default;

            virtual private_image_ptr data() const noexcept override { return m_data.get(); }

        private:
            std::unique_ptr<private_image_value[]> m_data;
        };

        dimension_t m_width;
        dimension_t m_height;
        std::shared_ptr<underlying> m_underlying;
        private_image_ptr m_data;
    };
}    // namespace randomcat::engine::graphics::textures
