#include <GL/glew.h>
#include <twurtle/detail/raii_wrappers/vbo_raii.h>

namespace randomcat::graphics::detail {
    struct vbo_id_wrapper::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteBuffers(1, &m_id); }
    };

    vbo_id_wrapper::vbo_id_wrapper() : m_ptr(nullptr) {}
    vbo_id_wrapper::vbo_id_wrapper(unsigned int _id) : m_ptr(std::make_shared<underlying>(_id)) {}

    unsigned int vbo_id_wrapper::id() const { return m_ptr->m_id; }
    vbo_id_wrapper::operator unsigned int() const { return id(); }
}    // namespace randomcat::graphics::detail
