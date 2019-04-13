#pragma once

#include <chrono>
#include <cstdint>

namespace randomcat::engine {
    class system_timer {
    public:
        using time = std::chrono::milliseconds;

        explicit system_timer(time _startTime) noexcept : m_startTime(_startTime) {}

        // Does not make sense to tick a temporary timer
        void tick(time _currentTime) & noexcept {
            m_lastTickTime = std::move(m_currentTickTime);
            m_currentTickTime = std::move(_currentTime);
            tick_fps();
        }

        [[nodiscard]] auto current_time() const noexcept { return m_currentTickTime; }

        [[nodiscard]] auto previous_time() const noexcept { return m_lastTickTime; }

        [[nodiscard]] auto delta_time() const noexcept { return current_time() - previous_time(); }

        [[nodiscard]] auto start_time() const noexcept { return m_startTime; }

        [[nodiscard]] auto fps() const noexcept { return m_ticksLastSecond; }

    private:
        time m_startTime;
        time m_lastTickTime = m_startTime;
        time m_currentTickTime = m_startTime;

        time m_lastFpsTime = m_startTime;
        std::int16_t m_ticksThisSecond = 0;
        std::int16_t m_ticksLastSecond = 0;

        void tick_fps() noexcept {
            using namespace std::chrono_literals;

            ++m_ticksThisSecond;

            if (current_time() > (m_lastFpsTime + 1s)) {
                m_lastFpsTime = current_time();
                m_ticksLastSecond = m_ticksThisSecond;
                m_ticksThisSecond = 0;
            }
        }
    };
}    // namespace randomcat::engine
