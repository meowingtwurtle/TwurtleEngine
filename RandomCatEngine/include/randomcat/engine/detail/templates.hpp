#pragma once

namespace randomcat::engine::util_detail {
    template<typename...>
    static auto constexpr invalid = false;
}
