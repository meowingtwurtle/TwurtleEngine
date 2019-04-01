#pragma once

#include <SDL2/SDL_video.h>

#include "randomcat/engine/detail/raii_active_lock.hpp"
#include "randomcat/engine/graphics/global_gl_calls.hpp"
#include "randomcat/engine/graphics/window.hpp"

namespace randomcat::engine::graphics {
    namespace render_context_detail {
        struct context_data {
            SDL_Window* window;
            SDL_GLContext context;
        };

        inline void activate_context(context_data _context) noexcept { SDL_GL_MakeCurrent(_context.window, _context.context); }

        inline context_data current_context() noexcept { return context_data{SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext()}; }

        struct render_context_init_error_tag {};
    }    // namespace render_context_detail

    using render_context_init_error = util_detail::tag_exception<render_context_detail::render_context_init_error_tag>;

    using render_context_active_lock = util_detail::basic_active_lock<render_context_detail::activate_context, render_context_detail::current_context>;

    class render_context {
    public:
        explicit render_context(window const& _window) noexcept(!"Throws on error");

        auto make_active_lock() const noexcept { return render_context_active_lock(m_context); }

        template<typename F, typename... Args>
        void render(F&& _f, Args&&... _args) const noexcept {
            auto l = make_active_lock();
            clear_graphics();
            std::forward<F>(_f)(std::forward<Args>(_args)...);
            swap_buffers();
        }

    private:
        void swap_buffers() const noexcept { SDL_GL_SwapWindow(m_context.window); }

        render_context_detail::context_data m_context;
    };

}    // namespace randomcat::engine::graphics
