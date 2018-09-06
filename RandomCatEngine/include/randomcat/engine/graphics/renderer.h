#pragma once

#include <vector>

namespace randomcat::engine::graphics {
    class renderer {
    public:
        virtual ~renderer() = default;
        virtual void render() const = 0;
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

    template<typename... Ts>
    class multi_renderer;

    template<>
    class multi_renderer<> : public renderer {
    public:
        virtual void render() const override {}
    };

    template<typename T, typename... Ts>
    class multi_renderer<T, Ts...> : private multi_renderer<Ts...> {
    public:
        multi_renderer(T _t, Ts... _others) : parent(_others...), m_item(std::move(_t)) {}

        virtual void render() const override {
            m_item.render();
            parent::render();
        }

    private:
        using parent = multi_renderer<Ts...>;

        T m_item;
    };
}    // namespace randomcat::engine::graphics
