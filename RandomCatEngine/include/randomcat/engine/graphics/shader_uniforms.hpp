#pragma once

namespace randomcat::engine::graphics {
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
}    // namespace randomcat::engine::graphics
