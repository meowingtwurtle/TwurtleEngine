#pragma once

#include <chrono>
#include <memory>

#include "randomcat/engine/controller_timer.hpp"
#include "randomcat/engine/graphics/global_gl_calls.hpp"
#include "randomcat/engine/graphics/window.hpp"
#include "randomcat/engine/init.hpp"
#include "randomcat/engine/input/input_state.hpp"

namespace randomcat::engine {
    class controller {
    public:
        controller() = default;

        controller(controller const&) = delete;
        controller(controller&&) = delete;

        [[nodiscard]] auto const& timer() const noexcept { return m_timer; }

        void tick() noexcept {
            fetch_raw_events();
            m_timer.tick(fetch_current_raw_time());
        }

        [[nodiscard]] auto const& inputs() const noexcept { return m_currentInputState; }
        [[nodiscard]] auto const& input_changes() const noexcept { return m_inputStateChanges; }

        [[nodiscard]] auto quit_received() const noexcept { return m_quitReceived; }

    private:
        input::input_state m_currentInputState;
        input::input_state_changes m_inputStateChanges;

        bool m_quitReceived = false;

        system_timer m_timer = system_timer{fetch_current_raw_time()};

        std::chrono::milliseconds fetch_current_raw_time() const noexcept;

        void fetch_raw_events() noexcept;
    };
}    // namespace randomcat::engine
