#pragma once

#include <memory>

namespace randomcat::graphics::detail {
    struct vbo_id_wrapper {
        vbo_id_wrapper();
        vbo_id_wrapper(unsigned int _id);

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::graphics::detail
