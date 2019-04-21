#include "randomcat/engine/low_level/graphics/render_context.hpp"

#include <GL/glew.h>

namespace randomcat::engine::graphics {
    namespace {
        std::string_view gl_debug_type_name(GLenum _type) noexcept {
            using namespace std::string_view_literals;

            switch (_type) {
                case GL_DEBUG_TYPE_ERROR: return "ERROR"sv;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOR"sv;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED BEHAVIOR"sv;
                case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY ERROR"sv;
                case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE ISSUE"sv;
                case GL_DEBUG_TYPE_MARKER: return "MARKER"sv;
                case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG INFO GROUP PUSH"sv;
                case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG INFO GROUP POP"sv;
                case GL_DEBUG_TYPE_OTHER: return "OTHER INFO"sv;
                default: return "UNRECOGNIZED GROUP"sv;
            }
        }

        std::string_view gl_debug_serverity_name(GLenum _severity) noexcept {
            using namespace std::string_view_literals;

            switch (_severity) {
                case GL_DEBUG_SEVERITY_HIGH: return "HIGH"sv;
                case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM"sv;
                case GL_DEBUG_SEVERITY_LOW: return "LOW"sv;
                case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION"sv;
                default: return "UNRECOGNIZED SEVERITY"sv;
            }
        }

        std::string_view gl_debug_source_name(GLenum _source) noexcept {
            using namespace std::string_view_literals;

            switch (_source) {
                case GL_DEBUG_SOURCE_API: return "API"sv;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOWING SYSTEM"sv;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER"sv;
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY"sv;
                case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION"sv;
                case GL_DEBUG_SOURCE_OTHER: return "OTHER"sv;
                default: return "UNRECOGNIZED SOURCE"sv;
            }
        }

        GLAPIENTRY void gl_log_callback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar* _message, const void* _userParam) {
            log::warn << "[GL DEBUG LOG INFO] "
                      << "[FROM " << gl_debug_source_name(_source) << "] [OF TYPE " << gl_debug_type_name(_type) << "] [WITH SEVERITY "
                      << gl_debug_serverity_name(_severity) << "]: " << std::string_view(_message, _length);
        }
    }    // namespace

    render_context::render_context(window const& _window, flags _flags) noexcept(false)
    : m_context{_window.raw_pointer(impl_call), SDL_GL_CreateContext(_window.raw_pointer(impl_call))} {
        auto l = make_active_lock();
        enable_depth_test();

        auto glewErr = glewInit();
        if (glewErr != GLEW_OK) {
            throw render_context_init_error{"Error initializing GLEW: " + std::string{reinterpret_cast<char const*>(glewGetErrorString(glewErr))}};
        }

        auto sdlFlags = static_cast<std::underlying_type_t<SDL_GLcontextFlag>>(SDL_GLcontextFlag());
        if (is_debug(_flags)) {
            sdlFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;

            log::info << "Context created with debugging";

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(gl_log_callback, nullptr);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, sdlFlags);
    }
}    // namespace randomcat::engine::graphics
