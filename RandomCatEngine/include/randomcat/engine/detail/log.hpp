#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace randomcat::engine::log {
    enum class log_type : uint8_t { INFO, WARN, ERROR };

    inline std::string log_type_name(log_type _logType) noexcept {
        switch (_logType) {
            case log_type::INFO: return "INFO";
            case log_type::WARN: return "WARN";
            case log_type::ERROR: return "ERROR";
        }

        return std::string{"Invalid log type: "} + std::to_string(static_cast<std::underlying_type_t<decltype(_logType)>>(_logType));
    }

    inline void log(std::string_view _message, log_type _type) noexcept {
        std::ostringstream fullMessage{};
        time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto formatTime = *std::localtime(&systemTime);
        fullMessage << "[" << std::put_time(&formatTime, "%Y-%m-%d %H:%M:%S") << "] [" << log_type_name(_type) << "] " << _message;

        std::clog << fullMessage.str() << '\n';
    }

    inline void info(std::string_view _message) noexcept { log(_message, log_type::INFO); }
    inline void warn(std::string_view _message) noexcept { log(_message, log_type::WARN); }
    inline void error(std::string_view _message) noexcept { log(_message, log_type::ERROR); }
}    // namespace randomcat::engine::log
