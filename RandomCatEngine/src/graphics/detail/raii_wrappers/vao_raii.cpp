#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/vao_raii.h>

namespace randomcat::engine::graphics::detail {
    struct vao_id::underlying {
        unsigned int m_id;
        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteVertexArrays(1, &m_id); }
    };

    vao_id::vao_id() {
        unsigned int id;
        glGenVertexArrays(1, &id);
        m_ptr = std::make_shared<underlying>(id);
    }

    unsigned int vao_id::id() const { return m_ptr->m_id; }
    vao_id::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail
