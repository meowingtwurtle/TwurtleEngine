#pragma once

namespace randomcat::engine::graphics {
    template<typename T>
    struct tag_t {};

    template<typename T>
    static constexpr auto tag = tag_t<T>{};
}    // namespace randomcat::engine::graphics
