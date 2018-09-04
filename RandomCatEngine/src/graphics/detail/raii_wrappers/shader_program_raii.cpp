#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.h>

namespace randomcat::engine::graphics::detail {
    struct shader_id_wrapper::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteShader(m_id); }
    };

    shader_id_wrapper::shader_id_wrapper() : m_ptr(nullptr) {}
    shader_id_wrapper::shader_id_wrapper(unsigned int _id) : m_ptr(std::make_shared<underlying>(_id)) {}

    unsigned int shader_id_wrapper::id() const { return m_ptr->m_id; }
    shader_id_wrapper::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail

namespace randomcat::engine::graphics::detail {
    struct program_id_wrapper::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteProgram(m_id); }
    };

    program_id_wrapper::program_id_wrapper() : m_ptr(nullptr) {}
    program_id_wrapper::program_id_wrapper(unsigned int _id) : m_ptr(std::make_shared<underlying>(_id)) {}

    unsigned int program_id_wrapper::id() const { return m_ptr->m_id; }
    program_id_wrapper::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail
