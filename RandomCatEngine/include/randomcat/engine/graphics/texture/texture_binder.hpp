#pragma once

#include <GL/glew.h>

#include "randomcat/engine/graphics/detail/gl_error_guard.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/texture_raii.hpp"
#include "randomcat/engine/graphics/texture/texture_manager.hpp"

namespace randomcat::engine::graphics::texture {
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

    private:
        using id_type = std::conditional_t<Shared, gl_raii_detail::shared_texture_id, gl_raii_detail::unique_texture_id>;

        explicit basic_texture_array(id_type _id, int _width, int _height, int _layers)
        : m_id(std::move(_id)), m_width(std::move(_width)), m_height(std::move(_height)), m_layers(std::move(_layers)) {}

        id_type m_id;
        int m_width;
        int m_height;
        int m_layers;

        template<bool, bool>
        friend class basic_texture_array;

        friend basic_texture_array<false, true> gen_texture_array(int, int, int) noexcept;

        // Not part of the public interface
        // Wrappers provided in namespace texture_array_detail

        friend int m_tex_arr_width(basic_texture_array const& _arr) { return _arr.m_width; }

        friend int m_tex_arr_height(basic_texture_array const& _arr) { return _arr.m_height; }

        friend int m_tex_arr_layers(basic_texture_array const& _arr) { return _arr.m_layers; }

        friend auto m_tex_arr_raw_id(basic_texture_array const& _arr) { return _arr.m_id.value(); }
    };

    namespace texture_array_detail {
        template<bool Shared, bool Mutable>
        auto layers(basic_texture_array<Shared, Mutable> const& _arr) {
            return m_tex_arr_layers(_arr);
        }

        template<bool Shared, bool Mutable>
        auto width(basic_texture_array<Shared, Mutable> const& _arr) {
            return m_tex_arr_width(_arr);
        }

        template<bool Shared, bool Mutable>
        auto height(basic_texture_array<Shared, Mutable> const& _arr) {
            return m_tex_arr_height(_arr);
        }

        template<bool Shared, bool Mutable>
        auto raw_id(basic_texture_array<Shared, Mutable> const& _arr) {
            return m_tex_arr_raw_id(_arr);
        }
    }    // namespace texture_array_detail

    using unique_texture_array = basic_texture_array</*Shared=*/false, true>;
    using shared_texture_array = basic_texture_array</*Shared=*/true, true>;
    using const_unique_texture_array = unique_texture_array::as_const;
    using const_shared_texture_array = shared_texture_array::as_const;

    unique_texture_array gen_texture_array(int _width, int _height, int _layers) noexcept {
        RC_GL_ERROR_GUARD("generating texture array");

        gl_raii_detail::unique_texture_id id;
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _width, _height, _layers);

        return unique_texture_array{std::move(id), _width, _height, _layers};
    }

    template<bool TextureIsShared>
    void bind_texture_array_layer(basic_texture_array<TextureIsShared, /*IsMutable=*/true> const& _array,
                                  texture_array_index _layerNum,
                                  texture const& _texture) noexcept {
        RC_GL_ERROR_GUARD("binding texture");

        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array_detail::raw_id(_array));
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
                        _texture.data());
    }
}    // namespace randomcat::engine::graphics::texture
