#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/detail/noexcept_util.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.hpp>
#include <randomcat/engine/graphics/shader_input.hpp>

namespace randomcat::engine::graphics {
    class shader {
    public:
        explicit shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) noexcept(false);

        RC_NOEXCEPT_CONSTRUCT_ASSIGN(shader);

        void make_active() const noexcept { make_active(m_programID); }

        std::vector<shader_input> const& inputs() const noexcept;

        bool operator==(shader const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader const& _other) const noexcept { return !(*this == _other); }

        class const_uniform_manager {
        public:
            explicit const_uniform_manager(detail::program_id _programID) : m_programID(std::move(_programID)) {}

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

        private:
            detail::program_id m_programID;

            class active_lock {
            public:
                active_lock(detail::program_id _programID) noexcept;
                ~active_lock() noexcept;

            private:
                static GLuint get_active_program() noexcept;
                static void set_active_program(GLuint _id) noexcept;

                std::optional<GLuint> m_oldID = std::nullopt;
                detail::program_id m_programID;
            };

        protected:
            active_lock make_active_lock() const noexcept { return active_lock(m_programID); }
        };

        class uniform_manager : public const_uniform_manager {
        public:
            explicit uniform_manager(detail::program_id _programID) : const_uniform_manager(std::move(_programID)) {}

            // You agree not to change that active program during calls to these
            // functions. These functions will re-activate the previous shader after
            // completion.

            void set_bool(std::string_view _name, bool _value) noexcept;
            void set_int(std::string_view _name, int _value) noexcept;
            void set_float(std::string_view _name, float _value) noexcept;
            void set_vec3(std::string_view _name, glm::vec3 const& _value) noexcept;
            void set_mat4(std::string_view _name, glm::mat4 const& _value) noexcept;
        };

        const_uniform_manager uniforms() const { return const_uniform_manager(m_programID); }
        uniform_manager uniforms() { return uniform_manager(m_programID); }

    private:
        detail::program_id m_programID;

        static void make_active(detail::program_id _program) noexcept;

        friend class shader_view;
    };

    class shader_view {
    public:
        // m_inputs is safe, the shader_inputs are stored in a global map, will not be
        // replaced until the program_id's value is reused, which the existence of
        // this prevents.

        shader_view(shader&& _other) : m_programID(std::move(_other.m_programID)), m_inputs(std::ref(_other.inputs())) {}

        shader_view(shader const& _other) : m_programID(_other.m_programID), m_inputs(std::ref(_other.inputs())) {}

        bool operator==(shader_view const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader_view const& _other) const noexcept { return !(*this == _other); }

        void make_active() const noexcept { shader::make_active(m_programID); }

        std::vector<shader_input> const& inputs() const noexcept { return m_inputs; }

        shader::const_uniform_manager uniforms() const noexcept { return shader::const_uniform_manager(m_programID); }

    private:
        detail::program_id m_programID;
        std::reference_wrapper<std::vector<shader_input> const> m_inputs;
    };
}    // namespace randomcat::engine::graphics
