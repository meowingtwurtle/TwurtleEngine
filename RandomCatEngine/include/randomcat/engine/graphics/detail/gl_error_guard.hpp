#pragma once

#include <string>
#include <string_view>

#include <GL/glew.h>

#include <randomcat/engine/detail/log.hpp>

namespace randomcat::engine::graphics::detail {
    class gl_error_guard {
    public:
        explicit gl_error_guard() : gl_error_guard("[UNKNOWN]") {}
        explicit gl_error_guard(std::string _doingWhat) : m_errorString("Error encountered while " + _doingWhat) {}

        gl_error_guard(gl_error_guard const&) = delete;
        gl_error_guard(gl_error_guard&&) = delete;

        ~gl_error_guard() {
            GLenum error;
            while ((error = glGetError()) != GL_NO_ERROR) { log::error << m_errorString << ": " << error_string(error) << "!"; }
        }

    private:
        static std::string_view error_string(GLenum _error) {
            switch (_error) {
                case GL_NO_ERROR: return "[no error]";
                case GL_INVALID_ENUM: return "Invalid enum passed to command";
                case GL_INVALID_VALUE: return "Invalid value passed to command";
                case GL_INVALID_OPERATION: return "Invalid operation";
                case GL_STACK_OVERFLOW: return "Command would cause stack overflow";
                case GL_STACK_UNDERFLOW: return "Command would cause stack underflow";
                case GL_OUT_OF_MEMORY: return "OpenGL has run out of memory";
                case GL_TABLE_TOO_LARGE: return "Table size too large";
            }

            return "[UNKNOWN ERROR]";
        }

        std::string m_errorString;
    };
}    // namespace randomcat::engine::graphics::detail

#define RC_GL_ERROR_GUARD_IMPL_Y(num, state) ::randomcat::engine::graphics::detail::gl_error_guard g##num(state);
#define RC_GL_ERROR_GUARD_IMPL_X(num, state) RC_GL_ERROR_GUARD_IMPL_Y(num, state)
#define RC_GL_ERROR_GUARD(state) RC_GL_ERROR_GUARD_IMPL_X(__COUNTER__, state)
