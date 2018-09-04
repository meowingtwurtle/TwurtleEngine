#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.h>

namespace randomcat::engine::graphics::detail {
    struct shader_id::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteShader(m_id); }
    };

    shader_id::shader_id(GLenum _type) : m_ptr(std::make_shared<underlying>(glCreateShader(_type))) {}

    unsigned int shader_id::id() const { return m_ptr->m_id; }
    shader_id::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail

namespace randomcat::engine::graphics::detail {
    struct program_id::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteProgram(m_id); }
    };

    program_id::program_id() : m_ptr(std::make_shared<underlying>(glCreateProgram())) {}

    unsigned int program_id::id() const { return m_ptr->m_id; }
    program_id::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail
