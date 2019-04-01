#pragma once

#include <type_traits>

namespace randomcat::engine::util_detail {
    template<auto Activate, auto Current>
    class basic_active_lock;

    template<typename IdType, void (*Activate)(IdType), IdType (*Current)()>
    class basic_active_lock<Activate, Current> {
    public:
        static_assert(std::is_trivial_v<IdType>);

        basic_active_lock(basic_active_lock const&) = delete;
        basic_active_lock(basic_active_lock&&) = delete;

        basic_active_lock(IdType _new) noexcept : m_old(Current()) { Activate(_new); }

        ~basic_active_lock() noexcept { Activate(m_old); }

    private:
        IdType m_old;
    };
}    // namespace randomcat::engine::util_detail
