#pragma once

#include <memory>

namespace randomcat::engine::graphics::gl_raii_detail {
    template<bool copyable, auto, auto, typename...>
    struct basic_opengl_raii_id;

    using opengl_raw_id = uint32_t;
    static_assert(std::is_integral_v<opengl_raw_id>);

    // Yes, an impl namespace within a detail namespace. Sue me.
    namespace impl {
        // Must be here so that deleter does not depend on is_shared.

        template<auto>
        struct deleter;

        template<bool _destroy_noexcept, void (*_destroy_id_f)(opengl_raw_id) noexcept(_destroy_noexcept)>
        struct deleter<_destroy_id_f> {
            deleter(opengl_raw_id _id) noexcept : m_id(_id) {}

            deleter(deleter const&) = delete;
            deleter(deleter&&) = delete;

            ~deleter() noexcept(_destroy_noexcept) { _destroy_id_f(m_id); }

            opengl_raw_id m_id;
        };
    }    // namespace impl

    template<bool _is_shared,
             typename... _construct_args_ts,
             typename... _create_args_ts,
             bool _create_noexcept,
             opengl_raw_id (*_create_id_f)(_create_args_ts...) noexcept(_create_noexcept),
             bool _destroy_noexcept,
             void (*_destroy_id_f)(opengl_raw_id) noexcept(_destroy_noexcept)>
    struct basic_opengl_raii_id<_is_shared, _create_id_f, _destroy_id_f, _construct_args_ts...> {
        using this_t = basic_opengl_raii_id;

        // Only delete if we require construct args
        template<bool Enable = sizeof...(_construct_args_ts), typename = std::enable_if_t<Enable>>
        basic_opengl_raii_id() = delete;

        explicit basic_opengl_raii_id(_construct_args_ts... _args) noexcept(_create_noexcept)
        // This depends on order of fields, as m_id is referenced in deleter
        // constructor rvalue unique_ptr is convertible to shared_ptr
        : m_id(_create_id_f(_args...)), m_deleter(std::make_unique<deleter>(m_id)) {}

        using as_unique = basic_opengl_raii_id<false, _create_id_f, _destroy_id_f, _construct_args_ts...>;
        using as_shared = basic_opengl_raii_id<true, _create_id_f, _destroy_id_f, _construct_args_ts...>;

        static auto constexpr is_shared = _is_shared;
        static auto constexpr is_unique = !is_shared;

        template<bool Enable = is_shared, typename = std::enable_if_t<Enable>>
        basic_opengl_raii_id(as_unique&& _other) : m_id(std::move(_other.m_id)), m_deleter(std::move(_other.m_deleter)) {}

        opengl_raw_id value() const noexcept { return m_id; }
        /* implicit */ operator opengl_raw_id() const { return value(); }

        bool operator==(this_t const& _other) const noexcept { return value() == _other.value(); }

        bool operator!=(this_t const& _other) const noexcept { return !(*this == _other); }

    private:
        // Do not reorder these fields, as constructor init-list depends on it

        opengl_raw_id m_id;

        using deleter = impl::deleter<_destroy_id_f>;

        std::conditional_t<_is_shared, std::shared_ptr<deleter>, std::unique_ptr<deleter>> m_deleter;

        template<bool copyable, auto, auto, typename...>
        friend struct basic_opengl_raii_id;
    };

    template<auto _create_id_f, auto _destroy_id_f, typename... _construct_args_ts>
    using unique_opengl_raii_id = basic_opengl_raii_id<false, _create_id_f, _destroy_id_f, _construct_args_ts...>;

    template<auto _create_id_f, auto _destroy_id_f, typename... _construct_args_ts>
    using shared_opengl_raii_id = basic_opengl_raii_id<true, _create_id_f, _destroy_id_f, _construct_args_ts...>;
}    // namespace randomcat::engine::graphics::gl_raii_detail
