#pragma once

#include <algorithm>
#include <optional>
#include <type_traits>

#include <randomcat/engine/detail/tag.hpp>
#include <randomcat/engine/graphics/detail/default_vertex.hpp>
#include <randomcat/engine/graphics/detail/default_vertex_renderer.hpp>
#include <randomcat/engine/graphics/object.hpp>

namespace randomcat::engine::graphics {
    template<typename _object_t, typename _sub_renderer_t, typename _transform_func_t, typename = std::void_t<std::invoke_result_t<_sub_renderer_t, std::invoke_result_t<_transform_func_t, _object_t>>>>
    class transform_object_renderer {
    public:
        using object = _object_t;
        using sub_renderer = _sub_renderer_t;

        explicit transform_object_renderer(sub_renderer _subRenderer, _transform_func_t _transform) noexcept(
            std::is_nothrow_move_constructible_v<sub_renderer>&& std::is_nothrow_move_constructible_v<_transform_func_t>)
        : m_subRenderer(std::move(_subRenderer)), m_transform(std::move(_transform)) {}

        explicit transform_object_renderer(tag_t<object>, sub_renderer _subRenderer, _transform_func_t _transform) noexcept(
            noexcept(transform_object_renderer(std::move(_subRenderer), std::move(_transform))))
        : transform_object_renderer(std::move(_subRenderer), std::move(_transform)) {}

        void operator()(object const& _object) const
            noexcept(noexcept(std::declval<sub_renderer>()(std::move(std::declval<_transform_func_t>()(_object))))) {
            m_subRenderer(std::move(m_transform(_object)));
        }

    private:
        using sub_object_extractor = _transform_func_t;

        sub_renderer m_subRenderer;
        sub_object_extractor m_transform;
    };

    template<typename _object_t, typename _sub_renderer_t, typename _sub_object_extractor_t = decltype(_object_t::sub_extractor_f)>
    static inline auto make_decomposing_renderer(_sub_renderer_t _subRenderer, _sub_object_extractor_t _subExtractor = _object_t::sub_extractor_f) noexcept(
        noexcept(transform_object_renderer<_object_t, _sub_renderer_t, _sub_object_extractor_t>(std::move(_subRenderer), std::move(_subExtractor)))) {
        return transform_object_renderer<_object_t, _sub_renderer_t, _sub_object_extractor_t>(std::move(_subRenderer), std::move(_subExtractor));
    }

    template<typename _object_t, typename _sub_renderer_t, typename _sub_object_extractor_t = decltype(_object_t::sub_extractor_f)>
    static inline auto make_decomposing_renderer(tag_t<_object_t>, _sub_renderer_t _subRenderer, _sub_object_extractor_t _subExtractor = _object_t::sub_extractor_f) noexcept(
        noexcept(make_decomposing_renderer<_object_t, _sub_renderer_t, _sub_object_extractor_t>(std::move(_subRenderer), std::move(_subExtractor)))) {
        return make_decomposing_renderer<_object_t, _sub_renderer_t, _sub_object_extractor_t>(std::move(_subRenderer), std::move(_subExtractor));
    }

    template<typename _object_t, typename _render_func_t, typename = std::void_t<std::invoke_result_t<_render_func_t, _object_t>>>
    class for_each_object_renderer {
    public:
        using object = _object_t;

        explicit for_each_object_renderer(_render_func_t _renderFunc) noexcept(std::is_nothrow_move_constructible_v<_render_func_t>)
        : m_renderFunc(std::move(_renderFunc)) {}

        template<typename... Ts>
        explicit for_each_object_renderer(tag_t<object>, _render_func_t _renderFunc) noexcept(noexcept(for_each_object_renderer(std::move(_renderFunc))))
        : for_each_object_renderer(std::move(_renderFunc)) {}

        void operator()(object const& _object) const noexcept(noexcept(m_renderFunc(_object))) { m_renderFunc(_object); }

        template<typename _container_t, typename = std::void_t<std::invoke_result_t<_render_func_t, decltype(*begin(std::declval<_container_t>()))>>>
        void operator()(_container_t const& _objects) const
            noexcept(noexcept(std::for_each(begin(_objects), end(_objects), std::declval<_render_func_t>()))) {
            std::for_each(begin(_objects), end(_objects), m_renderFunc);
        }

    private:
        _render_func_t m_renderFunc;
    };
}    // namespace randomcat::engine::graphics
