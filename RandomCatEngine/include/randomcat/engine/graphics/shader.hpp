#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.hpp>
#include <randomcat/engine/graphics/shader_input.hpp>

namespace randomcat::engine::graphics {
    namespace detail {
        void activate_program(detail::shared_program_id const& _program) noexcept;
    }

    class const_shader_uniform_manager {
    public:
        explicit const_shader_uniform_manager(detail::shared_program_id _programID) noexcept : m_programID(std::move(_programID)) {}

        // You agree not to change that active program during calls to these
        // functions. These functions will re-activate the previous shader after
        // completion.

        bool get_bool(std::string_view _name) const noexcept;
        int get_int(std::string_view _name) const noexcept;
        float get_float(std::string_view _name) const noexcept;
        glm::vec3 get_vec3(std::string_view _name) const noexcept;
        glm::mat4 get_mat4(std::string_view _name) const noexcept;

    protected:
        GLint get_uniform_location(std::string_view _name) const noexcept;
        detail::shared_program_id program() const noexcept { return m_programID; }

    private:
        detail::shared_program_id m_programID;

        class active_lock {
        public:
            active_lock(active_lock const&) = delete;
            active_lock(active_lock&&) = delete;

            active_lock(detail::shared_program_id const& _programID) noexcept;
            ~active_lock() noexcept;

        private:
            static GLuint get_active_program() noexcept;
            static void set_active_program(GLuint _id) noexcept;

            std::optional<GLuint> m_oldID = std::nullopt;
            detail::shared_program_id const& m_programID;
        };

    protected:
        active_lock make_active_lock() const noexcept { return active_lock(m_programID); }
    };

    class shader_uniform_manager : public const_shader_uniform_manager {
    public:
        explicit shader_uniform_manager(detail::shared_program_id _programID) noexcept
        : const_shader_uniform_manager(std::move(_programID)) {}

        // You agree not to change that active program during calls to these
        // functions. These functions will re-activate the previous shader after
        // completion.

        void set_bool(std::string_view _name, bool _value) noexcept;
        void set_int(std::string_view _name, int _value) noexcept;
        void set_float(std::string_view _name, float _value) noexcept;
        void set_vec3(std::string_view _name, glm::vec3 const& _value) noexcept;
        void set_mat4(std::string_view _name, glm::mat4 const& _value) noexcept;
    };

    template<typename Vertex>
    class shader {
    public:
        using vertex = Vertex;

        shader(shader const&) = delete;
        shader(shader&&) = default;

        explicit shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) noexcept(false);

        void make_active() const noexcept { detail::activate_program(m_programID); }

        std::vector<shader_input> inputs() const noexcept { return m_inputs; }

        bool operator==(shader const& _other) const noexcept { return program() == _other.program(); }
        bool operator!=(shader const& _other) const noexcept { return !(*this == _other); }

        shader_uniform_manager uniforms() { return shader_uniform_manager(program()); }
        decltype(auto) uniforms() const { return const_uniforms(); }

        const_shader_uniform_manager const_uniforms() const { return const_shader_uniform_manager(program()); }

        template<typename NewVertex>
        shader<NewVertex> reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept;

        shader<Vertex> reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept;

        template<typename NewVertex>
        shader<NewVertex> reinterpret_vertex() const noexcept;

        shader clone() const noexcept;

    protected:
        explicit shader(detail::shared_program_id _program, std::vector<shader_input> _inputs) noexcept
        : m_programID(std::move(_program)), m_inputs(std::move(_inputs)) {}

        detail::shared_program_id const& program() const noexcept { return m_programID; }

    private:
        detail::shared_program_id m_programID;
        std::vector<shader_input> m_inputs;

        template<typename>
        friend class shader_view;
    };

    template<typename Vertex>
    class shader_view {
    public:
        // m_inputs is safe, the shader_inputs are stored in a global map, will not be
        // replaced until the shared_program_id's value is reused, which the existence
        // of this prevents.

        shader_view(shader<Vertex> const& _other) : shader_view(_other.program(), _other.inputs()) {}

        bool operator==(shader_view const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader_view const& _other) const noexcept { return !(*this == _other); }

        void make_active() const noexcept { detail::activate_program(m_programID); }

        std::vector<shader_input> inputs() const noexcept { return m_inputs; }

        const_shader_uniform_manager uniforms() const noexcept { return const_shader_uniform_manager(m_programID); }
        decltype(auto) const_uniforms() const noexcept { return uniforms(); }

        template<typename NewVertex>
        shader<NewVertex> reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept;

        shader<Vertex> reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept;

        template<typename NewVertex>
        shader<NewVertex> reinterpret_vertex() const noexcept;

        shader<Vertex> clone() const noexcept;

    protected:
        explicit shader_view(detail::shared_program_id _program, std::vector<shader_input> _inputs)
        : m_programID(std::move(_program)), m_inputs(std::move(_inputs)) {}

        detail::shared_program_id const& program() { return m_programID; }

    private:
        detail::shared_program_id m_programID;
        std::vector<shader_input> m_inputs;
    };
}    // namespace randomcat::engine::graphics

#include <randomcat/engine/graphics/detail/shader_impl.inl>
