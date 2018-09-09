#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.h>
#include <randomcat/engine/graphics/shader_input.h>

namespace randomcat::engine::graphics {
    struct shader_compile_fail {};
    struct shader_link_fail {};

    class shader {
    public:
        shader();
        shader(char const* _vertex, char const* _fragment, std::vector<shader_input> _inputs);

        void makeActive() const;

        void setBool(std::string const& _name, bool _value);
        void setInt(std::string const& _name, int _value);
        void setFloat(std::string const& _name, float _value);
        void setVec3(std::string const& _name, glm::vec3 const& _value);
        void setMat4(std::string const& _name, glm::mat4 const& _value);

        std::vector<shader_input> inputs() const { return m_inputs; }

    private:
        detail::program_id m_programID;
        std::vector<shader_input> m_inputs;
    };
}    // namespace randomcat::engine::graphics
