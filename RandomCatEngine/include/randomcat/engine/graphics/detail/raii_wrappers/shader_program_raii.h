#pragma once

#include <memory>

namespace randomcat::engine::graphics::detail {
    struct shader_id_wrapper {
    public:
        shader_id_wrapper();
        shader_id_wrapper(unsigned int _id);

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };

    struct program_id_wrapper {
    public:
        program_id_wrapper();
        program_id_wrapper(unsigned int _id);

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::detail
