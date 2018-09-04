#include <GL/glew.h>

#include <randomcat/engine/graphics/detail/raii_wrappers/vbo_raii.h>

namespace randomcat::engine::graphics::detail {
    struct vbo_id::underlying {
        unsigned int m_id;

        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteBuffers(1, &m_id); }
    };

    vbo_id::vbo_id() {
        unsigned int id;
        glGenBuffers(1, &id);
        m_ptr = std::make_shared<underlying>(id);
    }

    unsigned int vbo_id::id() const { return m_ptr->m_id; }
    vbo_id::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::detail
