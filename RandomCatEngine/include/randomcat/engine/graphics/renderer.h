#pragma once

#include <vector>

namespace randomcat::engine::graphics {
    class renderer {
    public:
        virtual void render() const = 0;
        virtual void makeActive() const = 0;
    };

    template<typename _vertex_t>
    class vertex_renderer : public renderer {
    public:
        virtual std::vector<_vertex_t>& vertices() = 0;
        virtual std::vector<_vertex_t> const& vertices() const = 0;
    };
}    // namespace randomcat::engine::graphics
