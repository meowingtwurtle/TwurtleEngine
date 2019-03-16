#pragma once

#include <string>

#include <SDL2/SDL_keyboard.h>

#define RC_KC_DEF_SDL(symbol, sdl_name) kc_##symbol = SDLK_##sdl_name
#define RC_KC_DEF(symbol, sdl_name) RC_KC_DEF_SDL(symbol, sdl_name)
#define RC_KC_DEF_SAME(symbol) RC_KC_DEF(symbol, symbol)

namespace randomcat::engine {
    namespace input {
        enum class keycode : SDL_Keycode {
            RC_KC_DEF_SAME(a),
            RC_KC_DEF_SAME(b),
            RC_KC_DEF_SAME(c),
            RC_KC_DEF_SAME(d),
            RC_KC_DEF_SAME(e),
            RC_KC_DEF_SAME(f),
            RC_KC_DEF_SAME(g),
            RC_KC_DEF_SAME(h),
            RC_KC_DEF_SAME(i),
            RC_KC_DEF_SAME(j),
            RC_KC_DEF_SAME(k),
            RC_KC_DEF_SAME(l),
            RC_KC_DEF_SAME(m),
            RC_KC_DEF_SAME(n),
            RC_KC_DEF_SAME(o),
            RC_KC_DEF_SAME(p),
            RC_KC_DEF_SAME(q),
            RC_KC_DEF_SAME(r),
            RC_KC_DEF_SAME(s),
            RC_KC_DEF_SAME(t),
            RC_KC_DEF_SAME(u),
            RC_KC_DEF_SAME(v),
            RC_KC_DEF_SAME(w),
            RC_KC_DEF_SAME(x),
            RC_KC_DEF_SAME(y),
            RC_KC_DEF_SAME(z),
            RC_KC_DEF(space, SPACE),
            RC_KC_DEF(comma, COMMA),
            RC_KC_DEF(up, UP),
            RC_KC_DEF(down, DOWN),
            RC_KC_DEF(left, LEFT),
            RC_KC_DEF(right, RIGHT),
            RC_KC_DEF(escape, ESCAPE),
            RC_KC_DEF(lshift, LSHIFT),
            RC_KC_DEF(rshift, RSHIFT),
        };
    }

    namespace input_detail {
        [[nodiscard]] inline auto raw_key(input::keycode _key) noexcept { return static_cast<SDL_Keycode>(_key); }

        [[nodiscard]] inline auto wrap_key(SDL_Keycode _key) noexcept { return static_cast<input::keycode>(_key); }

        struct no_such_keycode_error_tag {};
    }    // namespace input_detail

    namespace input {
        using no_such_keycode_error = util_detail::tag_exception<input_detail::no_such_keycode_error_tag>;

        [[nodiscard]] inline keycode keycode_from_name(std::string const& _character) {
            auto const sdlKey = SDL_GetKeyFromName(_character.c_str());
            if (sdlKey == SDLK_UNKNOWN) throw no_such_keycode_error("Invalid keycode name: " + _character);

            return input_detail::wrap_key(sdlKey);
        }

        [[nodiscard]] inline std::string name_from_keycode(keycode _keycode) noexcept {
            return SDL_GetKeyName(input_detail::raw_key(_keycode));
        }
    }    // namespace input
}    // namespace randomcat::engine

#undef RC_KC_DEF_SDL
#undef RC_KC_DEF_SAME
#undef RC_KC_DEF
