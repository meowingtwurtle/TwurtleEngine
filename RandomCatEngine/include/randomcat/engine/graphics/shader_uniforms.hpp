#pragma once

#include <glm/mat4x4.hpp>

#include <randomcat/type_container/type_list.hpp>

#include "randomcat/engine/detail/tag_exception.hpp"
#include "randomcat/engine/detail/templates.hpp"
#include "randomcat/engine/graphics/detail/gl_types.hpp"
#include "randomcat/engine/graphics/detail/gl_wrappers.hpp"

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
        struct no_such_uniform_error_tag {};
    }    // namespace shader_detail

    using no_such_uniform_error = util_detail::tag_exception<shader_detail::no_such_uniform_error_tag>;

    template<typename Capabilities = uniform_no_capabilities>
    class shader_uniform_reader {
    public:
        static_assert(shader_detail::valid_capabilities<Capabilities>, "Capabilities must be valid");

        explicit shader_uniform_reader(gl_detail::shared_program_id _programID) noexcept : m_programID(std::move(_programID)) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ shader_uniform_reader(shader_uniform_reader<OtherCapabilities> const& _other) noexcept
        : shader_uniform_reader(_other.program()) {}

        template<typename T>
        static inline constexpr auto has_capability = type_container::type_list_contains_v<Capabilities, T>;

        // These functions will re-activate the previous shader after completion.
        // These functions will throw shader_no_such_uniform_error if the referenced
        // uniform does not exist

        [[nodiscard]] bool get_bool(std::string const& _name) const noexcept(!"Throws if uniform not found");
        [[nodiscard]] GLint get_int(std::string const& _name) const noexcept(!"Throws if uniform not found");
        [[nodiscard]] GLfloat get_float(std::string const& _name) const noexcept(!"Throws if uniform not found");
        [[nodiscard]] glm::vec3 get_vec3(std::string const& _name) const noexcept(!"Throws if uniform not found");
        [[nodiscard]] glm::mat4 get_mat4(std::string const& _name) const noexcept(!"Throws if uniform not found");

        template<typename Wrapper, typename = std::enable_if_t<has_capability<Wrapper>>>
        [[nodiscard]] Wrapper as() const noexcept(noexcept(Wrapper(*this))) {
            return Wrapper(*this);
        }

        template<typename Func,
                 typename = std::enable_if_t<type_container::type_list_contains_v<Capabilities, decltype(std::forward<Func>(std::declval<std::add_lvalue_reference_t<Func&&>>())(std::declval<shader_uniform_reader const&>()))>>>
        [[nodiscard]] decltype(auto) as(Func&& _func) const noexcept(noexcept(std::forward<Func>(_func)(*this))) {
            return std::forward<Func>(_func)(*this);
        }

    protected:
        [[nodiscard]] GLint get_uniform_location(std::string const& _name) const noexcept(!"Throws if uniform not found");
        [[nodiscard]] auto const& program() const noexcept { return m_programID; }

    private:
        gl_detail::shared_program_id m_programID;

    protected:
        auto make_active_lock() const noexcept { return gl_detail::program_lock(m_programID); }

        template<typename>
        friend class shader_uniform_reader;
    };

    template<typename Capabilities = uniform_no_capabilities>
    class shader_uniform_writer : public shader_uniform_reader<Capabilities> {
    public:
        static_assert(shader_detail::valid_capabilities<Capabilities>, "Capabilities must be valid");

        explicit shader_uniform_writer(gl_detail::shared_program_id _programID) noexcept
        : shader_uniform_reader<Capabilities>(std::move(_programID)) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ shader_uniform_writer(shader_uniform_writer<OtherCapabilities> const& _other)
        : shader_uniform_writer(_other.program()) {}

        template<typename OtherCapabilities, typename = std::enable_if_t<type_container::type_list_is_sub_list_of_v<Capabilities, OtherCapabilities>>>
        /* implicit */ operator shader_uniform_reader<OtherCapabilities>() const {
            return shader_uniform_reader<OtherCapabilities>(this->program());
        }

        template<typename T>
        static inline constexpr auto has_capability = type_container::type_list_contains_v<Capabilities, T>;

        // These functions will re-activate the previous shader after completion.
        // These functions will throw shader_no_such_uniform_error if the referenced
        // uniform does not exist

        void set_bool(std::string const& _name, bool _value) const noexcept(!"Throws if uniform not found");
        void set_int(std::string const& _name, GLint _value) const noexcept(!"Throws if uniform not found");
        void set_float(std::string const& _name, GLfloat _value) const noexcept(!"Throws if uniform not found");
        void set_vec3(std::string const& _name, glm::tvec3<GLfloat> const& _value) const noexcept(!"Throws if uniform not found");
        void set_mat4(std::string const& _name, glm::tmat4x4<GLfloat> const& _value) const noexcept(!"Throws if uniform not found");

        template<typename Wrapper, typename = std::enable_if_t<has_capability<Wrapper>>>
        [[nodiscard]] Wrapper as() const noexcept(noexcept(Wrapper(*this))) {
            return Wrapper(*this);
        }

        template<typename Func,
                 typename = std::enable_if_t<has_capability<decltype(std::forward<Func>(std::declval<std::add_lvalue_reference_t<Func>>())(std::declval<shader_uniform_writer const&>()))>>>
        [[nodiscard]] decltype(auto) as(Func&& _func) const noexcept(noexcept(std::forward<Func>(_func)(*this))) {
            return std::forward<Func>(_func)(*this);
        }

        template<typename>
        friend class shader_uniform_writer;
    };

    namespace uniform_manager_detail {
        template<typename T>
        struct uniform_manager_capabilities_s {
            static_assert(util_detail::invalid<T>, "Argument must be a uniform manager");
        };

        template<typename Capabilities>
        struct uniform_manager_capabilities_s<shader_uniform_reader<Capabilities>> {
            using type = Capabilities;
        };

        template<typename Capabilities>
        struct uniform_manager_capabilities_s<shader_uniform_writer<Capabilities>> {
            using type = Capabilities;
        };
    }    // namespace uniform_manager_detail

    template<typename UniformManager>
    using uniform_manager_capabilities = typename uniform_manager_detail::uniform_manager_capabilities_s<UniformManager>::type;
}    // namespace randomcat::engine::graphics
