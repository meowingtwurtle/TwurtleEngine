#pragma once

#include <GL/glew.h>

#include "randomcat/engine/detail/impl_only_access.hpp"
#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/texture_raii.hpp"
#include "randomcat/engine/graphics/texture/texture_manager.hpp"

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

        friend basic_texture_array<false, true> gen_texture_array(int, int, int) noexcept;
    };

    using unique_texture_array = basic_texture_array</*Shared=*/false, true>;
    using shared_texture_array = basic_texture_array</*Shared=*/true, true>;
    using const_unique_texture_array = unique_texture_array::as_const;
    using const_shared_texture_array = shared_texture_array::as_const;

    [[nodiscard]] unique_texture_array gen_texture_array(GLsizei _width, GLsizei _height, GLsizei _layers) noexcept {
        RC_GL_ERROR_GUARD("generating texture array");

        gl_detail::unique_texture_id id;
        glBindTexture(GL_TEXTURE_2D_ARRAY, id.value());
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _width, _height, _layers);

        return unique_texture_array{std::move(id), _width, _height, _layers};
    }

    template<bool TextureIsShared>
    void bind_texture_array_layer(basic_texture_array<TextureIsShared, /*IsMutable=*/true> const& _array,
                                  texture_array_index _layerNum,
                                  texture const& _texture) noexcept {
        RC_GL_ERROR_GUARD("binding texture");

        glBindTexture(GL_TEXTURE_2D_ARRAY, _array.raw_id(impl_call).value);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                        0,
                        0,
                        0,
                        _layerNum.value,
                        _texture.width(),
                        _texture.height(),
                        1,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        _texture.data(impl_call));
    }
}    // namespace randomcat::engine::graphics::textures
