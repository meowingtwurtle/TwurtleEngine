#pragma once

#include <memory>

namespace randomcat::engine::graphics::detail {
    template<auto, auto, typename...>
    struct opengl_raii_id;

    using opengl_raw_id = uint32_t;
    static_assert(std::is_integral_v<opengl_raw_id>);

    template<typename... _construct_args_ts,
             typename... _create_args_ts,
             bool _create_noexcept,
             opengl_raw_id (*_create_id_f)(_create_args_ts...) noexcept(_create_noexcept),
             bool _destroy_noexcept,
             void (*_destroy_id_f)(opengl_raw_id) noexcept(_destroy_noexcept)>
    struct opengl_raii_id<_create_id_f, _destroy_id_f, _construct_args_ts...> {
        using this_t = opengl_raii_id;

        // Only delete if we require construct args
        template<bool Enable = sizeof...(_construct_args_ts), typename = std::enable_if_t<Enable>>
        opengl_raii_id() = delete;

        explicit opengl_raii_id(_construct_args_ts... _args) noexcept(_create_noexcept)
        // This depends on order of fields, as m_id is referenced in deleter
        // constructor
        : m_id(_create_id_f(_args...)), m_deleter(std::make_shared<deleter>(m_id)) {}

        opengl_raw_id value() const noexcept { return m_id; }
        /* implicit */ operator opengl_raw_id() const { return value(); }

        bool operator==(this_t const& _other) const noexcept { return value() == _other.value(); }

        bool operator!=(this_t const& _other) const noexcept { return !(*this == _other); }

    private:
        // Do not reorder these fields, as constructor init-list depends on it

        opengl_raw_id m_id;

        struct deleter {
            deleter(opengl_raw_id _id) noexcept : m_id(_id) {}

            ~deleter() noexcept(_destroy_noexcept) { _destroy_id_f(m_id); }

            opengl_raw_id m_id;
        };

        std::shared_ptr<deleter> m_deleter;
    };
}    // namespace randomcat::engine::graphics::detail
