#pragma once

#include <memory>

namespace randomcat::engine::graphics::texture::detail {
    struct texture_id {
        texture_id();

        unsigned int id() const;
        operator unsigned int() const;

        bool operator==(texture_id const& _other) const { return id() == _other.id(); }
        bool operator!=(texture_id const& _other) const { return !(*this == _other); }

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::texture::detail
