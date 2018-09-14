#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.h>
#include <randomcat/engine/graphics/shader_input.h>

namespace randomcat::engine::graphics {
    class shader {
    public:
        shader();
        shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs);

        void make_active() const;

        void uniform_set_bool(std::string const& _name, bool _value);
        void uniform_set_int(std::string const& _name, int _value);
        void uniform_set_float(std::string const& _name, float _value);
        void uniform_set_vec3(std::string const& _name, glm::vec3 const& _value);
        void uniform_set_mat4(std::string const& _name, glm::mat4 const& _value);

        std::vector<shader_input> inputs() const;

        bool operator==(shader const& _other) const { return m_programID == _other.m_programID; }
        bool operator!=(shader const& _other) const { return !(*this == _other); }

    private:
        detail::program_id m_programID;
    };
}    // namespace randomcat::engine::graphics
