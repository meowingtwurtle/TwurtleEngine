#pragma once

#include <cstdint>
#include <utility>

#include <GL/glew.h>

#include "randomcat/engine/detail/safe_int.hpp"

namespace randomcat::engine::graphics {
    enum class shader_input_attribute_base_type : char {
        floating_point,
        integral,
    };

    namespace shader_input_detail {
        struct shader_input_attribute_count_tag {};
    }    // namespace shader_input_detail

    using shader_input_attribute_size = util_detail::safe_integer<std::int16_t, shader_input_detail::shader_input_attribute_count_tag>;

    class shader_input_attribute_type {
    public:
        /* implicit */ constexpr shader_input_attribute_type(shader_input_attribute_base_type _baseType, shader_input_attribute_size _size) noexcept
        : m_baseType(std::move(_baseType)), m_size(std::move(_size)) {}

        auto base() const noexcept { return m_baseType; }

        auto size() const noexcept { return m_size; }

    private:
        shader_input_attribute_base_type m_baseType;
        shader_input_attribute_size m_size;
    };

    enum class shader_input_storage_type {
        floating_point = GL_FLOAT,
        signed_int = GL_INT,
        unsigned_int = GL_UNSIGNED_INT,
        signed_byte = GL_BYTE,
        unsigned_byte = GL_UNSIGNED_BYTE,
    };

    namespace shader_input_detail {
        struct input_index_tag {};
        struct input_offset_tag {};
        struct input_stride_tag {};
    }    // namespace shader_input_detail

    using shader_input_index = util_detail::safe_integer<std::int8_t, shader_input_detail::input_index_tag>;
    using shader_input_offset = util_detail::safe_integer<std::int16_t, shader_input_detail::input_offset_tag>;
    using shader_input_stride = util_detail::safe_integer<std::int16_t, shader_input_detail::input_stride_tag>;

    struct shader_input {
        static constexpr auto densely_packed = shader_input_stride{0};

#define RC_INPUT_TYPE(name, base, count)                                                                                                   \
    static auto constexpr name##_type = shader_input_attribute_type{shader_input_attribute_base_type::base, shader_input_attribute_size{count}};

        RC_INPUT_TYPE(float, floating_point, 1);
        RC_INPUT_TYPE(vec2, floating_point, 2);
        RC_INPUT_TYPE(vec3, floating_point, 3);
        RC_INPUT_TYPE(vec4, floating_point, 4);

        RC_INPUT_TYPE(int, integral, 1);
        RC_INPUT_TYPE(ivec2, integral, 2);
        RC_INPUT_TYPE(ivec3, integral, 3);
        RC_INPUT_TYPE(ivec4, integral, 4);

#undef RC_INPUT_TYPE

        shader_input_index index;
        shader_input_attribute_type attributeType;
        shader_input_storage_type storageType;
        shader_input_offset offset;
        shader_input_stride stride;
    };
}    // namespace randomcat::engine::graphics
