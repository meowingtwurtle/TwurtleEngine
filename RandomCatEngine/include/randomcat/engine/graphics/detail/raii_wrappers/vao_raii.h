#pragma once

#include <memory>

namespace randomcat::engine::graphics::detail {
    struct vao_id_wrapper {
        vao_id_wrapper();
        vao_id_wrapper(unsigned int _id);

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::detail
