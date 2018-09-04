#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.h>

namespace randomcat::engine::graphics::detail {
    struct vao_id_wrapper::underlying {
        unsigned int m_id;
        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteVertexArrays(1, &m_id); }
    };

    vao_id_wrapper::vao_id_wrapper() {
        unsigned int id;
        glGenVertexArrays(1, &id);
        m_ptr = std::make_shared<underlying>(id);
    }

    unsigned int vao_id_wrapper::id() const { return m_ptr->m_id; }
    vao_id_wrapper::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail
