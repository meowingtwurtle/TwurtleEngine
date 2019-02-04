#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <randomcat/type_container/type_list.hpp>

#include "randomcat/engine/detail/tag_exception.hpp"
#include "randomcat/engine/graphics/detail/raii_wrappers/shader_program_raii.hpp"
#include "randomcat/engine/graphics/shader_input.hpp"

namespace randomcat::engine::graphics {
    namespace shader_detail {
        void activate_program(gl_raii_detail::shared_program_id const& _program) noexcept;
    }

    namespace shader_detail {
        struct shader_no_such_uniform_error_tag {};
    }    // namespace shader_detail

    using shader_no_such_uniform_error = util_detail::tag_exception<shader_detail::shader_no_such_uniform_error_tag>;

    namespace shader_detail {
        template<typename Capabilities>
        static auto constexpr valid_capabilities =
            type_container::is_type_list_v<Capabilities> && not type_container::type_list_has_duplicates_v<Capabilities>;
    }

    template<typename... Ts>
    using uniform_capabilities = type_container::type_list<Ts...>;

    using uniform_no_capabilities = type_container::empty_type_list;

    namespace shader_detail {
        class program_active_lock {
        public:
            program_active_lock(program_active_lock const&) = delete;
            program_active_lock(program_active_lock&&) = delete;

            explicit program_active_lock(gl_raii_detail::shared_program_id const& _programID) noexcept;
            ~program_active_lock() noexcept;

        private:
            static GLuint get_active_program() noexcept;
            static void set_active_program(GLuint _id) noexcept;

            std::optional<GLuint> m_oldID = std::nullopt;
            gl_raii_detail::shared_program_id const& m_programID;
        };
    }    // namespace shader_detail

    template<typename Capabilities = uniform_no_capabilities>
    class shader_uniform_reader {
    public:
        static_assert(shader_detail::valid_capabilities<Capabilities>, "Capabilities must be valid");

        explicit shader_uniform_reader(gl_raii_detail::shared_program_id _programID) noexcept : m_programID(std::move(_programID)) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ shader_uniform_reader(shader_uniform_reader<OtherCapabilities> const& _other)
        : shader_uniform_reader(_other.program()) {}

        // These functions will re-activate the previous shader after completion.
        // These functions will throw shader_no_such_uniform_error if the referenced
        // uniform does not exist

        bool get_bool(std::string const& _name) const noexcept(!"Throws if uniform not found");
        int get_int(std::string const& _name) const noexcept(!"Throws if uniform not found");
        float get_float(std::string const& _name) const noexcept(!"Throws if uniform not found");
        glm::vec3 get_vec3(std::string const& _name) const noexcept(!"Throws if uniform not found");
        glm::mat4 get_mat4(std::string const& _name) const noexcept(!"Throws if uniform not found");

        template<typename Wrapper>
        Wrapper as() const noexcept(noexcept(Wrapper(*this))) {
            return Wrapper(*this);
        }

    protected:
        GLint get_uniform_location(std::string const& _name) const noexcept(!"Throws if uniform not found");
        gl_raii_detail::shared_program_id program() const noexcept { return m_programID; }

    private:
        gl_raii_detail::shared_program_id m_programID;

    protected:
        auto make_active_lock() const noexcept { return shader_detail::program_active_lock(m_programID); }

        template<typename>
        friend class shader_uniform_reader;
    };

    template<typename Capabilities = uniform_no_capabilities>
    class shader_uniform_writer : public shader_uniform_reader<Capabilities> {
    public:
        static_assert(shader_detail::valid_capabilities<Capabilities>, "Capabilities must be valid");

        explicit shader_uniform_writer(gl_raii_detail::shared_program_id _programID) noexcept
        : shader_uniform_reader<Capabilities>(std::move(_programID)) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ shader_uniform_writer(shader_uniform_writer<OtherCapabilities> const& _other)
        : shader_uniform_writer(_other.program()) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ operator shader_uniform_reader<OtherCapabilities>() const {
            return shader_uniform_reader<OtherCapabilities>(this->program());
        }

        // These functions will re-activate the previous shader after completion.
        // These functions will throw shader_no_such_uniform_error if the referenced
        // uniform does not exist

        void set_bool(std::string const& _name, bool _value) const noexcept(!"Throws if uniform not found");
        void set_int(std::string const& _name, int _value) const noexcept(!"Throws if uniform not found");
        void set_float(std::string const& _name, float _value) const noexcept(!"Throws if uniform not found");
        void set_vec3(std::string const& _name, glm::vec3 const& _value) const noexcept(!"Throws if uniform not found");
        void set_mat4(std::string const& _name, glm::mat4 const& _value) const noexcept(!"Throws if uniform not found");

        template<typename Wrapper>
        Wrapper as() const noexcept(noexcept(Wrapper(*this))) {
            return Wrapper(*this);
        }

        template<typename>
        friend class shader_uniform_writer;
    };

    namespace shader_detail {
        struct shader_init_error_tag {};
    }    // namespace shader_detail
    using shader_init_error = util_detail::tag_exception<shader_detail::shader_init_error_tag>;

    template<typename... Ts>
    using shader_capabilities = uniform_capabilities<Ts...>;

    using shader_no_capabilities = uniform_no_capabilities;

    template<typename Vertex, typename UniformCapabilities = shader_no_capabilities>
    class shader {
    public:
        static_assert(shader_detail::valid_capabilities<UniformCapabilities>, "UniformCapabilities must be valid");

        using vertex = Vertex;
        using uniform_manager = shader_uniform_writer<UniformCapabilities>;
        using const_uniform_manager = shader_uniform_writer<UniformCapabilities>;

        shader(shader const&) = delete;
        shader(shader&&) noexcept = default;

        shader& operator=(shader&&) noexcept = default;

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<UniformCapabilities, OtherCapabilities>>>
        /* implicit */ shader(shader<Vertex, OtherCapabilities>&& _other) noexcept
        : m_programID(std::move(_other.program())), m_inputs(std::move(_other.inputs())) {}

        explicit shader(std::string_view _vertex, std::string_view _fragment, std::vector<shader_input> _inputs) noexcept(!"Throws on error");

        void make_active() const noexcept { shader_detail::activate_program(m_programID); }

        std::vector<shader_input> inputs() const noexcept { return m_inputs; }

        bool operator==(shader const& _other) const noexcept { return program() == _other.program(); }
        bool operator!=(shader const& _other) const noexcept { return !(*this == _other); }

        uniform_manager uniforms() noexcept { return uniform_manager(program()); }
        decltype(auto) uniforms() const noexcept { return const_uniforms(); }

        const_uniform_manager const_uniforms() const noexcept { return shader_uniform_reader(program()); }

        template<typename NewVertex>
        shader<NewVertex, UniformCapabilities> reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept;

        shader<Vertex, UniformCapabilities> reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept;

        template<typename NewVertex>
        shader<NewVertex, UniformCapabilities> reinterpret_vertex() const noexcept;

        shader clone() const noexcept;

        template<typename UniformWrapper, typename = std::enable_if_t<type_container::type_list_contains_v<UniformCapabilities, UniformWrapper>>>
        UniformWrapper uniforms_as() noexcept(noexcept(UniformWrapper(uniforms()))) {
            return uniforms().template as<UniformWrapper>();
        }

        template<typename UniformWrapper, typename = std::enable_if_t<type_container::type_list_contains_v<UniformCapabilities, UniformWrapper>>>
        UniformWrapper uniforms_as() const noexcept(noexcept(const_uniforms().template as<UniformWrapper>())) {
            return const_uniforms().template as<UniformWrapper>();
        }

    protected:
        explicit shader(gl_raii_detail::shared_program_id _program, std::vector<shader_input> _inputs) noexcept
        : m_programID(std::move(_program)), m_inputs(std::move(_inputs)) {}

        gl_raii_detail::shared_program_id program() const noexcept { return m_programID; }

    private:
        gl_raii_detail::shared_program_id m_programID;
        std::vector<shader_input> m_inputs;

        template<typename, typename>
        friend class shader;

        template<typename, typename>
        friend class shader_view;
    };

    template<typename Vertex, typename UniformCapabilities = shader_no_capabilities>
    class shader_view {
    public:
        static_assert(shader_detail::valid_capabilities<UniformCapabilities>, "UniformCapabilities must be valid");

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<UniformCapabilities, OtherCapabilities>>>
        /* implicit */ shader_view(shader<Vertex, OtherCapabilities> const& _other) noexcept(!"Copying vector")
        : shader_view(_other.program(), _other.inputs()) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<UniformCapabilities, OtherCapabilities>>>
        /* implicit */ shader_view(shader_view<Vertex, OtherCapabilities> const& _other) noexcept(!"Copying vector")
        : shader_view(_other.program(), _other.inputs()) {}

        bool operator==(shader_view const& _other) const noexcept { return m_programID == _other.m_programID; }
        bool operator!=(shader_view const& _other) const noexcept { return !(*this == _other); }

        void make_active() const noexcept { shader_detail::activate_program(m_programID); }

        std::vector<shader_input> inputs() const noexcept { return m_inputs; }

        using const_uniform_manager = shader_uniform_writer<UniformCapabilities>;

        const_uniform_manager uniforms() const noexcept { return shader_uniform_reader(m_programID); }
        decltype(auto) const_uniforms() const noexcept { return uniforms(); }

        template<typename NewVertex>
        shader<NewVertex, UniformCapabilities> reinterpret_vertex_and_inputs(std::vector<shader_input> _inputs) const noexcept;

        shader<Vertex, UniformCapabilities> reinterpret_inputs(std::vector<shader_input> _inputs) const noexcept;

        template<typename NewVertex>
        shader<NewVertex, UniformCapabilities> reinterpret_vertex() const noexcept;

        shader<Vertex, UniformCapabilities> clone() const noexcept;

        template<typename UniformManager, typename = std::enable_if_t<type_container::type_list_contains_v<UniformCapabilities, UniformManager>>>
        UniformManager uniforms_as() const noexcept(noexcept(const_uniforms().template as<UniformManager>())) {
            return const_uniforms().template as<UniformManager>();
        }

    protected:
        explicit shader_view(gl_raii_detail::shared_program_id _program, std::vector<shader_input> _inputs) noexcept(!"Copying vector")
        : m_programID(std::move(_program)), m_inputs(std::move(_inputs)) {}

        gl_raii_detail::shared_program_id program() const noexcept { return m_programID; }

    private:
        gl_raii_detail::shared_program_id m_programID;
        std::vector<shader_input> m_inputs;
    };
}    // namespace randomcat::engine::graphics

#include "randomcat/engine/graphics/detail/shader_impl.inl"
