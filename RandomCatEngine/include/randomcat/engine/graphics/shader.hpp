#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/detail/noexcept_util.hpp>
#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.hpp>
#include <randomcat/engine/graphics/shader_input.hpp>

namespace randomcat::engine::graphics {
    class shader {
    public:
        shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) noexcept(false);

        RC_NOEXCEPT_CONSTRUCT_ASSIGN(shader);

        void make_active() const noexcept;

        std::vector<shader_input> const& inputs() const noexcept;

        bool operator==(shader const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader const& _other) const noexcept { return !(*this == _other); }

        class uniform_manager {
        public:
            explicit uniform_manager(detail::program_id _programID) : m_programID(std::move(_programID)) {}

            void set_bool(std::string const& _name, bool _value) noexcept;
            void set_int(std::string const& _name, int _value) noexcept;
            void set_float(std::string const& _name, float _value) noexcept;
            void set_vec3(std::string const& _name, glm::vec3 const& _value) noexcept;
            void set_mat4(std::string const& _name, glm::mat4 const& _value) noexcept;

        private:
            detail::program_id m_programID;
            GLint get_uniform_location(std::string const& _name) const;
            void make_active() const noexcept;

            class active_lock {
            public:
                active_lock(detail::program_id _programID);
                ~active_lock();

            private:
                static GLuint get_active_program();
                static void set_active_program(GLuint _id);

                std::optional<GLuint> m_oldID = std::nullopt;
                detail::program_id m_programID;
            };
        };

        uniform_manager uniforms() { return uniform_manager(m_programID); }

    private:
        detail::program_id m_programID;
    };
}    // namespace randomcat::engine::graphics
