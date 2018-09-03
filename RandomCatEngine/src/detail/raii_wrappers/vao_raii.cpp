#include <GL/glew.h>
#include <twurtle/detail/raii_wrappers/vao_raii.h>

namespace randomcat::graphics::detail {
    struct vao_id_wrapper::underlying {
        unsigned int m_id;
        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteVertexArrays(1, &m_id); }
    };

    vao_id_wrapper::vao_id_wrapper() : m_ptr(nullptr) {}
    vao_id_wrapper::vao_id_wrapper(unsigned int _id) : m_ptr(std::make_shared<underlying>(_id)) {}

    unsigned int vao_id_wrapper::id() const { return m_ptr->m_id; }
    vao_id_wrapper::operator unsigned int() const { return id(); }
}    // namespace randomcat::graphics::detail
