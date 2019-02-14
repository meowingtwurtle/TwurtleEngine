#pragma once

#include <cstddef>
#include <cstdint>

namespace randomcat::engine::util_detail {
    template<typename Tag>
    struct safe_integer {
        std::int32_t value;

        bool operator==(safe_integer const& _other) const noexcept { return value == _other.value; }

        bool operator!=(safe_integer const& _other) const noexcept { return !(*this == _other); }
    };
}    // namespace randomcat::engine::util_detail
