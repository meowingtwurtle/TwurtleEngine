#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <gsl/gsl_util>

#include "randomcat/engine/detail/log.hpp"
#include "randomcat/engine/graphics/detail/gl_context.hpp"
#include "randomcat/engine/graphics/window.hpp"
#include "randomcat/engine/init.hpp"

namespace randomcat::engine::graphics {
    class window {
    public:
        explicit window(std::string const& _title = "Twurtle Engine", std::int16_t _width = 600, std::int16_t _height = 600) noexcept;

        ~window() noexcept;

        window(window const&) = delete;
        window(window&&) = delete;

        struct dimensions {
            std::int16_t width;
            std::int16_t height;
        };

        void set_size(std::int16_t _width, std::int16_t _height) noexcept { set_size({_width, _height}); }
        void set_size(dimensions _dims) noexcept;

        [[nodiscard]] dimensions size() const noexcept;

        [[nodiscard]] auto width() const noexcept { return size().width; }

        [[nodiscard]] auto height() const noexcept { return size().height; }

        [[nodiscard]] auto aspect_ratio() const noexcept {
            dimensions dims = size();
            return float(dims.width) / float(dims.height);
        }

        [[nodiscard]] std::string title() const noexcept;

        void set_title(std::string const& _title) noexcept;

        void swap_buffers() noexcept;

        void set_cursor_shown(bool _shown) noexcept;

    private:
        void* m_window;

        friend void randomcat::engine::graphics::gl_detail::set_render_context(window const&);
    };
}    // namespace randomcat::engine::graphics
