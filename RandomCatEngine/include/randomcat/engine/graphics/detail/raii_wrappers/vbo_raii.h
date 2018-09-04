#pragma once

#include <memory>

namespace randomcat::engine::graphics::detail {
    struct vbo_id {
        vbo_id();

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::detail
