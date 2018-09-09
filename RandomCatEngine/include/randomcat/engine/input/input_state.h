#pragma once

#include <unordered_map>

#include <randomcat/engine/input/keycodes.h>

namespace randomcat::engine::input {
    class input_state {
    public:
        void update();

        bool keyIsDown(keycode _key) const {
            // map operator[] default-constructs value if absent. Since 0-value of
            // key_state is up, if there is no entry, this will be false.
            auto const it = m_map.find(_key);

            return it != m_map.end() && it->second != key_state::up;
        }

        bool keyIsUp(keycode _key) const {
            auto const it = m_map.find(_key);

            return it != m_map.end() && it->second == key_state::up;
        }
        bool keyIsHeld(keycode _key) const {
            auto const it = m_map.find(_key);

            return it != m_map.end() && it->second == key_state::held;
        }

        void keyDown(keycode _key) {
            auto& state = m_map[_key];

            // Must check here, since there are two down states: down and held.
            if (state == key_state::up) state = key_state::down;
        }

        void keyUp(keycode _key) { m_map[_key] = key_state::up; }

        int mouseX() const { return m_mouseX; }
        int mouseY() const { return m_mouseY; }

        int mouseRelX() const { return m_relMouseX; }
        int mouseRelY() const { return m_relMouseY; }

        bool shouldQuit() const { return m_shouldQuit; }

    private:
        void downToHeld() {
            for (auto& entry : m_map) {
                if (entry.second == key_state::down) entry.second = key_state::held;
            }
        }

        enum class key_state { up, down, held };

        int m_mouseX = 0;
        int m_mouseY = 0;

        int m_relMouseX = 0;
        int m_relMouseY = 0;

        bool m_shouldQuit = false;

        std::unordered_map<keycode, key_state> m_map;
    };
}    // namespace randomcat::engine::input
