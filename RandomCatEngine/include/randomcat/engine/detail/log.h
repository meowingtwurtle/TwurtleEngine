#pragma once

#include <string>

namespace randomcat::engine::log {
    enum class LogType : uint8_t { INFO, WARN, ERROR };

    void log(std::string const& _message, LogType _type) noexcept;

    inline void info(std::string const& _message) noexcept { log(_message, LogType::INFO); }
    inline void warn(std::string const& _message) noexcept { log(_message, LogType::WARN); }
    inline void error(std::string const& _message) noexcept { log(_message, LogType::ERROR); }

    inline std::string log_type_name(LogType _logType) noexcept;
}    // namespace randomcat::engine::log
