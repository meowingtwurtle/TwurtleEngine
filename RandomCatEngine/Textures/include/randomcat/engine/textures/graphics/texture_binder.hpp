#pragma once

#include <utility>

#include <GL/glew.h>

#include "randomcat/engine/low_level/detail/impl_only_access.hpp"
#include "randomcat/engine/low_level/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/low_level/graphics/gl_wrappers/texture_raii.hpp"
#include "randomcat/engine/textures/graphics/texture_manager.hpp"
#include "randomcat/engine/textures/graphics/texture_sections.hpp"

namespace randomcat::engine::graphics::textures {
    // Because this is designed to be a semi-user-facing class, we provide
    // mutability template arg as well as shared template arg
    template<bool Shared, bool IsMutable>
    class basic_texture_array {
    public:
        using as_unique = basic_texture_array<false, IsMutable>;
        using as_shared = basic_texture_array<true, IsMutable>;
        static auto constexpr is_shared = Shared;
        static auto constexpr is_unique = !is_shared;
        using as_const = basic_texture_array<Shared, false>;
        using as_mutable = basic_texture_array<Shared, true>;
        static auto constexpr is_mutable = IsMutable;
        static auto constexpr is_const = !is_mutable;

        basic_texture_array() = delete;

        template<bool Enable = is_shared, typename = std::enable_if_t<Enable>>
        /* implicit */ basic_texture_array(as_unique&& _other)
        : m_id(std::move(_other.m_id)), m_width(std::move(_other.m_width)), m_height(std::move(_other.m_height)), m_layers(std::move(_other.m_layers)) {}

        // Enable must be a parameter type to prevent error for copy constructor not
        // taking reference arg
        template<bool Enable = is_const>
        /* implicit */ basic_texture_array(std::enable_if_t<Enable, as_mutable> _other)
        : m_id(std::move(_other.m_id)), m_width(std::move(_other.m_width)), m_height(std::move(_other.m_height)), m_layers(std::move(_other.m_layers)) {}

        auto width(impl_call_only) const noexcept { return m_width; }

        auto height(impl_call_only) const noexcept { return m_height; }

        auto layers(impl_call_only) const noexcept { return m_layers; }

        auto raw_id(impl_call_only) const noexcept { return typename id_type::raw_id(m_id); }

    private:
        using id_type = std::conditional_t<Shared, gl_detail::shared_texture_id, gl_detail::unique_texture_id>;

        explicit basic_texture_array(id_type _id, GLsizei _width, GLsizei _height, GLsizei _layers)
        : m_id(std::move(_id)), m_width(std::move(_width)), m_height(std::move(_height)), m_layers(std::move(_layers)) {}

        id_type m_id;
        GLsizei m_width;
        GLsizei m_height;
        GLsizei m_layers;

        template<bool, bool>
        friend class basic_texture_array;

        friend basic_texture_array<false, true> make_texture_array(int _width, int _height, int _layers) noexcept;
    };

    using unique_texture_array = basic_texture_array</*Shared=*/false, true>;
    using shared_texture_array = basic_texture_array</*Shared=*/true, true>;
    using const_unique_texture_array = unique_texture_array::as_const;
    using const_shared_texture_array = shared_texture_array::as_const;

    [[nodiscard]] unique_texture_array make_texture_array(GLsizei _width, GLsizei _height, GLsizei _layers) noexcept {
        RC_GL_ERROR_GUARD("generating texture array");

        gl_detail::unique_texture_id id;
        glBindTexture(GL_TEXTURE_2D_ARRAY, id.value());
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _width, _height, _layers);

        return unique_texture_array{std::move(id), _width, _height, _layers};
    }

    template<bool TextureIsShared>
    texture_rectangle bind_texture_array_layer(basic_texture_array<TextureIsShared, /*IsMutable=*/true> const& _array,
                                               texture_array_index _layerNum,
                                               texture const& _texture) noexcept {
        RC_GL_ERROR_GUARD("binding texture");

        auto const imageWidth = _texture.width();
        auto const imageHeight = _texture.height();

        auto const textureArrayWidth = _array.width(impl_call);
        auto const textureArrayHeight = _array.height(impl_call);

        glBindTexture(GL_TEXTURE_2D_ARRAY, _array.raw_id(impl_call).value);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, _layerNum.value, imageWidth, imageHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, _texture.data(impl_call));

        return texture_rectangle{_layerNum,
                                 texture_rectangle::from_corner_and_dimensions,
                                 {0, 0},
                                 float(imageWidth) / float(textureArrayWidth),
                                 float(imageHeight) / float(textureArrayHeight)};
    }

    namespace texture_array_detail {
        template<typename To, typename... Ignored>
        using to_first = To;

        template<typename... Textures, std::size_t... Numbers>
        std::tuple<unique_texture_array, texture_array_detail::to_first<texture_rectangle, Textures>...> make_texture_array_from_layers_helper(
            std::index_sequence<Numbers...>,
            Textures const&... _textures) noexcept {
            static_assert((std::is_same_v<Textures, textures::texture> && ...), "All arguments must be textures");

            auto width = std::max({_textures.width()...});
            auto height = std::max({_textures.height()...});
            auto layers = sizeof...(Textures);
            auto array = make_texture_array(width, height, layers);

            return std::apply(
                [&](auto... textureSections) {
                    return std::tuple<unique_texture_array, texture_array_detail::to_first<texture_rectangle, Textures>...>{std::move(array),
                                                                                                                            std::move(textureSections)...};
                },
                std::forward_as_tuple(bind_texture_array_layer(array, {Numbers}, _textures)...));
        }
    }    // namespace texture_array_detail

    template<typename... Args>
    decltype(auto) make_texture_array_from_layers(Args&&... _args) noexcept(
        noexcept(texture_array_detail::make_texture_array_from_layers_helper(std::index_sequence_for<Args...>{}, std::forward<Args>(_args)...))) {
        return texture_array_detail::make_texture_array_from_layers_helper(std::index_sequence_for<Args...>{}, std::forward<Args>(_args)...);
    }
}    // namespace randomcat::engine::graphics::textures
