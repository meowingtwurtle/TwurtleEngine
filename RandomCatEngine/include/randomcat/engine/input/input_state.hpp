#pragma once

#include <set>
#include <unordered_map>
#include <unordered_set>

#include "randomcat/engine/input/keycodes.hpp"

namespace randomcat::engine::input {
    enum class key_state { up, down };

    class keyboard_input_state_changes {
    private:
        std::unordered_map<keycode, key_state> m_newStates;
        using key_iter_t = typename decltype(m_newStates)::iterator;
        using key_const_iter_t = typename decltype(m_newStates)::const_iterator;

        key_iter_t key_iter(keycode _key) noexcept { return m_newStates.find(_key); }

        key_const_iter_t key_iter(keycode _key) const noexcept { return m_newStates.find(_key); }

        bool key_was_changed(key_iter_t _iter) const noexcept { return _iter != end(m_newStates); }

        bool key_was_changed(key_const_iter_t _iter) const noexcept { return _iter != end(m_newStates); }

        friend class keyboard_input_state;

    public:
        std::unordered_set<keycode> changed_keys() const noexcept {
            std::unordered_set<keycode> changedKeys;
            std::transform(begin(m_newStates), end(m_newStates), std::inserter(changedKeys, begin(changedKeys)), [](auto const& node) {
                return node.first;
            });
            return changedKeys;
        }

        void set_key_up(keycode _key) noexcept { set_key_state(_key, key_state::up); }
        void set_key_down(keycode _key) noexcept { set_key_state(_key, key_state::down); }

        void set_key_state(keycode _key, key_state _state) noexcept { m_newStates[_key] = _state; }

        bool key_went_to(keycode _key, key_state _state) const noexcept {
            auto it = key_iter(_key);
            return key_was_changed(it) && (it->second == _state);
        }

        auto key_went_up(keycode _key) const noexcept { return key_went_to(_key, key_state::up); }

        auto key_went_down(keycode _key) const noexcept { return key_went_to(_key, key_state::down); }

        auto key_was_changed(keycode _key) const noexcept { return key_was_changed(key_iter(_key)); }

        auto key_new_state(keycode _key) const noexcept { return m_newStates.at(_key); }
    };

    // Note: key_state is presumed to be "up" until set.
    class keyboard_input_state {
    public:
        [[nodiscard]] auto key_is_down(keycode _key) const noexcept { return get_key_state(_key) == key_state::down; }

        [[nodiscard]] auto key_is_up(keycode _key) const noexcept { return get_key_state(_key) == key_state::up; }

        [[nodiscard]] key_state get_key_state(keycode _key) const noexcept {
            auto const it = m_map.find(_key);

            if (it != m_map.end()) {
                return it->second;
            } else {
                return key_state::up;
            }
        }

        void update(keyboard_input_state_changes const& _changes) {
            std::for_each(begin(_changes.m_newStates), end(_changes.m_newStates), [&, this](auto const& changeNode) {
                m_map[changeNode.first] = changeNode.second;
            });
        }

    private:
        std::unordered_map<keycode, key_state> m_map;
    };

    class mouse_input_state_changes {
    public:
        auto& delta_x() noexcept { return m_relX; }
        auto& delta_y() noexcept { return m_relY; }

        auto delta_x() const noexcept { return m_relX; }
        auto delta_y() const noexcept { return m_relY; }

    private:
        std::int16_t m_relX = 0;
        std::int16_t m_relY = 0;
    };

    class mouse_input_state {
    public:
        auto x() const noexcept { return m_mouseX; }
        auto y() const noexcept { return m_mouseY; }

        void update(mouse_input_state_changes const& _changes) noexcept {
            m_mouseX += _changes.delta_x();
            m_mouseY += _changes.delta_y();
        }

    private:
        std::int16_t m_mouseX = 0;
        std::int16_t m_mouseY = 0;
    };

    class input_state_changes {
    public:
        auto& mouse() noexcept { return m_mouseChanges; }
        auto const& mouse() const noexcept { return m_mouseChanges; }

        auto& keyboard() noexcept { return m_keyboardChanges; }
        auto const& keyboard() const noexcept { return m_keyboardChanges; }

    private:
        mouse_input_state_changes m_mouseChanges;
        keyboard_input_state_changes m_keyboardChanges;
    };

    class input_state {
    public:
        mouse_input_state& mouse() noexcept { return m_mouseState; }
        mouse_input_state const& mouse() const noexcept { return m_mouseState; }

        keyboard_input_state& keyboard() noexcept { return m_keyState; }
        keyboard_input_state const& keyboard() const noexcept { return m_keyState; }

        void update(input_state_changes const& _changes) noexcept {
            m_mouseState.update(_changes.mouse());
            m_keyState.update(_changes.keyboard());
        }

        void update(mouse_input_state_changes const& _changes) noexcept { m_mouseState.update(_changes); }

        void update(keyboard_input_state_changes const& _changes) noexcept { m_keyState.update(_changes); }

    private:
        mouse_input_state m_mouseState;
        keyboard_input_state m_keyState;
    };
}    // namespace randomcat::engine::input
