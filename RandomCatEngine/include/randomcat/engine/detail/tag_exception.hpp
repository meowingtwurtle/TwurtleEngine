#pragma once

namespace randomcat::engine::util_detail {
    template<typename Tag>
    struct tag_exception : std::exception {
    public:
        explicit tag_exception(std::string _error) noexcept : m_error(std::move(_error)) {}

        char const* what() const noexcept override { return m_error.c_str(); }

    private:
        std::string m_error;
    };
}    // namespace randomcat::engine::util_detail
