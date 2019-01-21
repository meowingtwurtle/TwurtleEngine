#pragma once

#include <unordered_map>

#include <randomcat/engine/input/keycodes.hpp>

namespace randomcat::engine::input {
    class input_state {
    public:
        enum class key_state { up, down, held };

        bool key_is_down(keycode _key) const noexcept {
            auto keyState = get_key_state(_key);
            return keyState == key_state::down || keyState == key_state::held;
        }

        bool key_is_up(keycode _key) const noexcept {
            auto keyState = get_key_state(_key);
            return keyState == std::nullopt || keyState == key_state::up;
        }

        bool key_is_held(keycode _key) const noexcept { return get_key_state(_key) == key_state::held; }

        std::optional<key_state> get_key_state(keycode _key) const noexcept {
            auto const it = m_map.find(_key);

            if (it != m_map.end()) {
                return it->second;
            } else {
                return std::nullopt;
            }
        }

        void set_key_down(keycode _key) noexcept {
            auto& state = m_map[_key];

            // Must check here, since there are two down states: down and held.
            if (state == key_state::up) state = key_state::down;
        }

        void set_key_up(keycode _key) noexcept { m_map[_key] = key_state::up; }

        int& mouse_x() noexcept { return m_mouseX; }
        int& mouse_y() noexcept { return m_mouseY; }

        int mouse_x() const noexcept { return m_mouseX; }
        int mouse_y() const noexcept { return m_mouseY; }

        int& rel_mouse_x() noexcept { return m_relMouseX; }
        int& rel_mouse_y() noexcept { return m_relMouseY; }

        int rel_mouse_x() const noexcept { return m_relMouseX; }
        int rel_mouse_y() const noexcept { return m_relMouseY; }

        void down_to_held() noexcept {
            for (auto&& entry : m_map) {
                if (entry.second == key_state::down) entry.second = key_state::held;
            }
        }

    private:
        int m_mouseX = 0;
        int m_mouseY = 0;

        int m_relMouseX = 0;
        int m_relMouseY = 0;

        std::unordered_map<keycode, key_state> m_map;
    };
}    // namespace randomcat::engine::input
