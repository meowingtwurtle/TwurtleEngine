#pragma once

#include <memory>

#include <GL/glew.h>

namespace randomcat::engine::graphics::detail {
    struct shader_id {
    public:
        shader_id(GLenum _type);

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };

    struct program_id {
    public:
        program_id();

        unsigned int id() const;
        operator unsigned int() const;

    private:
        struct underlying;
        std::shared_ptr<underlying> m_ptr;
    };
}    // namespace randomcat::engine::graphics::detail
