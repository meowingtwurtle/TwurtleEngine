#pragma once

#include <unordered_map>

#include "randomcat/engine/input/keycodes.hpp"

namespace randomcat::engine::input {
    // Note: key_state is presumed to be "up" until set.
    class keyboard_input_state {
    public:
        enum class key_state { up, down, held };

        bool key_is_down(keycode _key) const noexcept {
            auto keyState = get_key_state(_key);
            return keyState == key_state::down || keyState == key_state::held;
        }

        bool key_is_up(keycode _key) const noexcept {
            auto keyState = get_key_state(_key);
            return keyState == key_state::up;
        }

        bool key_is_held(keycode _key) const noexcept { return get_key_state(_key) == key_state::held; }

        key_state get_key_state(keycode _key) const noexcept {
            auto const it = m_map.find(_key);

            if (it != m_map.end()) {
                return it->second;
            } else {
                return key_state::up;
            }
        }

        void set_key_down(keycode _key) noexcept {
            auto& state = m_map[_key];

            // Must check here, since there are two down states: down and held.
            if (state == key_state::up) state = key_state::down;
        }

        void set_key_up(keycode _key) noexcept { m_map[_key] = key_state::up; }

        void down_to_held() noexcept {
            for (auto& entry : m_map) {
                if (entry.second == key_state::down) entry.second = key_state::held;
            }
        }

    private:
        std::unordered_map<keycode, key_state> m_map;
    };

    class mouse_input_state {
    public:
        int& x() noexcept { return m_mouseX; }
        int& y() noexcept { return m_mouseY; }

        int x() const noexcept { return m_mouseX; }
        int y() const noexcept { return m_mouseY; }

    private:
        int m_mouseX = 0;
        int m_mouseY = 0;
    };

    class relative_mouse_input_state {
    public:
        int& rel_x() noexcept { return m_relX; }
        int& rel_y() noexcept { return m_relY; }

        int rel_x() const noexcept { return m_relX; }
        int rel_y() const noexcept { return m_relY; }

    private:
        int m_relX = 0;
        int m_relY = 0;
    };

    class input_state {
    public:
        mouse_input_state& mouse_state() noexcept { return m_mouseState; }
        mouse_input_state mouse_state() const noexcept { return m_mouseState; }

        relative_mouse_input_state& rel_mouse_state() noexcept { return m_relMouseState; }
        relative_mouse_input_state rel_mouse_state() const noexcept { return m_relMouseState; }

        keyboard_input_state& keyboard_state() noexcept { return m_keyState; }
        keyboard_input_state keyboard_state() const noexcept { return m_keyState; }

    private:
        mouse_input_state m_mouseState;
        relative_mouse_input_state m_relMouseState;
        keyboard_input_state m_keyState;
    };
}    // namespace randomcat::engine::input
