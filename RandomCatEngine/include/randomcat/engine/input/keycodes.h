#pragma once

#include <string>

#include <SDL2/SDL.h>

#define RC_KC_DEF(symbol, sdl_name)                                                                                                        \
    static constexpr keycode const keycode_##symbol = SDLK_##sdl_name;                                                                     \
    static constexpr auto const kc_##symbol = keycode_##symbol;

#define RC_KC_DEF_SAME(symbol) RC_KC_DEF(symbol, symbol)

namespace randomcat::engine::input {
    namespace keycodes {
        using keycode = SDL_Keycode;

        RC_KC_DEF_SAME(a)
        RC_KC_DEF_SAME(b)
        RC_KC_DEF_SAME(c)
        RC_KC_DEF_SAME(d)
        RC_KC_DEF_SAME(e)
        RC_KC_DEF_SAME(f)
        RC_KC_DEF_SAME(g)
        RC_KC_DEF_SAME(h)
        RC_KC_DEF_SAME(i)
        RC_KC_DEF_SAME(j)
        RC_KC_DEF_SAME(k)
        RC_KC_DEF_SAME(l)
        RC_KC_DEF_SAME(m)
        RC_KC_DEF_SAME(n)
        RC_KC_DEF_SAME(o)
        RC_KC_DEF_SAME(p)
        RC_KC_DEF_SAME(q)
        RC_KC_DEF_SAME(r)
        RC_KC_DEF_SAME(s)
        RC_KC_DEF_SAME(t)
        RC_KC_DEF_SAME(u)
        RC_KC_DEF_SAME(v)
        RC_KC_DEF_SAME(w)
        RC_KC_DEF_SAME(x)
        RC_KC_DEF_SAME(y)
        RC_KC_DEF_SAME(z)
        RC_KC_DEF(space, SPACE)
        RC_KC_DEF(comma, COMMA)
        RC_KC_DEF(up, UP)
        RC_KC_DEF(down, DOWN)
        RC_KC_DEF(left, LEFT)
        RC_KC_DEF(right, RIGHT)
        RC_KC_DEF(escape, ESCAPE)
        RC_KC_DEF(lshift, LSHIFT)
        RC_KC_DEF(rshift, RSHIFT)

        inline keycode keycode_from_name(std::string const& _character) { return SDL_GetKeyFromName(_character.c_str()); }

        inline std::string name_from_keycode(keycode _keycode) { return SDL_GetKeyName(_keycode); }
    }    // namespace keycodes

    using keycode = keycodes::keycode;
}    // namespace randomcat::engine::input

#undef RC_KC_DEF_SAME
#undef RC_KC_DEF
