#pragma once

#include <vector>

namespace randomcat::engine::graphics {
    class renderer {
    public:
        virtual ~renderer() = default;
        virtual void render() const = 0;
        virtual void makeActive() const = 0;
    };

    // Using macro to highlight that replacements are the same thing, just with
    // different words

#define RENDERER_SPEC(singular, plural)                                                                                                    \
    template<typename _##singular##_t>                                                                                                     \
    class singular##_renderer : public renderer {                                                                                          \
    public:                                                                                                                                \
        virtual std::vector<_##singular##_t>& plural() = 0;                                                                                \
        virtual std::vector<_##singular##_t> const& plural() const = 0;                                                                    \
    };

    RENDERER_SPEC(vertex, vertices)
    RENDERER_SPEC(object, objects)

#undef RENDERER_SPEC
}    // namespace randomcat::engine::graphics
