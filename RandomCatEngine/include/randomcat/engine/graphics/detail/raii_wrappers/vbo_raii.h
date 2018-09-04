#pragma once

#include <memory>

namespace randomcat::engine::graphics::detail {
    struct vbo_id {
        vbo_id();

        unsigned int id() const;
        operator unsigned int() const;

        bool operator==(vbo_id const& _other) const { return id() == _other.id(); }
        bool operator!=(vbo_id const& _other) const { return !(*this == _other); }

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::detail
