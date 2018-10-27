#pragma once

#include <memory>
//#include <GL/glew.h>

namespace randomcat::engine::graphics::detail {
    template<auto, auto, typename...>
    struct opengl_raii_id;

    using opengl_raw_id = uint32_t;

    template<typename... _construct_args_ts, typename... _create_args_ts, opengl_raw_id (*_create_id_f)(_create_args_ts...), void (*_destroy_id_f)(opengl_raw_id)>
    struct opengl_raii_id<_create_id_f, _destroy_id_f, _construct_args_ts...> {
        using this_t = opengl_raii_id;

        explicit opengl_raii_id(_construct_args_ts... _args)
        // This depends on order of fields, as m_id is referenced in deleter
        // constructor
        : m_id(_create_id_f(_args...)), m_deleter(std::make_shared<deleter>(m_id)) {}

        opengl_raw_id value() const { return m_id; }
        /* implicit */ operator opengl_raw_id() const { return value(); }

        bool operator==(this_t const& _other) const { return value() == _other.value(); }

        bool operator!=(this_t const& _other) const { return !(*this == _other); }

    private:
        // Do not reorder these fields, as constructor init-list depends on it

        opengl_raw_id m_id;

        struct deleter {
            deleter(opengl_raw_id _id) : m_id(_id) {}

            ~deleter() { _destroy_id_f(m_id); }

            opengl_raw_id m_id;
        };

        std::shared_ptr<deleter> m_deleter;
    };
}    // namespace randomcat::engine::graphics::detail
