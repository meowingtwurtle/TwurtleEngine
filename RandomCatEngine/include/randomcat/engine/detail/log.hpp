#pragma once

#include <string>

namespace randomcat::engine::log {
    enum class LogType : uint8_t { INFO, WARN, ERROR };

    void log(std::string_view _message, LogType _type) noexcept;

    inline void info(std::string_view _message) noexcept { log(_message, LogType::INFO); }
    inline void warn(std::string_view _message) noexcept { log(_message, LogType::WARN); }
    inline void error(std::string_view _message) noexcept { log(_message, LogType::ERROR); }

    inline std::string log_type_name(LogType _logType) noexcept;
}    // namespace randomcat::engine::log
