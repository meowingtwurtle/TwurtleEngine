#include <GL/glew.h>

#include <randomcat/engine/graphics/texture/detail/texture_raii.h>

namespace randomcat::engine::graphics::texture::detail {
    struct texture_id::underlying {
        unsigned int m_id;
        underlying(unsigned int _id) : m_id(_id) {}
        ~underlying() { glDeleteTextures(1, &m_id); }
    };

    texture_id::texture_id() {
        unsigned int id;
        glGenTextures(1, &id);
        m_ptr = std::make_shared<underlying>(id);
    }

    unsigned int texture_id::id() const { return m_ptr->m_id; }
    texture_id::operator unsigned int() const { return id(); }
}    // namespace randomcat::engine::graphics::texture::detail
