#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>

namespace randomcat::engine::util_detail {
    template<typename Value, typename Tag>
    struct safe_integer {
        Value value;

        template<typename = decltype(std::declval<Value const&>() == std::declval<Value const&>())>
        [[nodiscard]] bool operator==(safe_integer const& _other) const noexcept {
            return value == _other.value;
        }

        template<typename = decltype(std::declval<Value const&>() != std::declval<Value const&>())>
        [[nodiscard]] bool operator!=(safe_integer const& _other) const noexcept {
            return value != _other.value;
        }

        template<typename = decltype(std::declval<std::ostream&>() << std::declval<Value const&>())>
        [[nodiscard]] friend std::ostream& operator<<(std::ostream& _out, safe_integer const& _val) noexcept {
            _out << _val.value;
            return _out;
        }
    };
}    // namespace randomcat::engine::util_detail
