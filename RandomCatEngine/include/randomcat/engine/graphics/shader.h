#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/detail/noexcept_util.h>
#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.h>
#include <randomcat/engine/graphics/shader_input.h>

namespace randomcat::engine::graphics {
    class shader {
    public:
        shader() noexcept(false);
        shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs) noexcept(false);

        RC_NOEXCEPT_CONSTRUCT_ASSIGN(shader);

        void make_active() const noexcept;

        void uniform_set_bool(std::string const& _name, bool _value) noexcept;
        void uniform_set_int(std::string const& _name, int _value) noexcept;
        void uniform_set_float(std::string const& _name, float _value) noexcept;
        void uniform_set_vec3(std::string const& _name, glm::vec3 const& _value) noexcept;
        void uniform_set_mat4(std::string const& _name, glm::mat4 const& _value) noexcept;

        std::vector<shader_input> const& inputs() const noexcept;

        bool operator==(shader const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader const& _other) const noexcept { return !(*this == _other); }

    private:
        detail::program_id m_programID;
    };
}    // namespace randomcat::engine::graphics
