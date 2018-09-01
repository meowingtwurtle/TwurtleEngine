#pragma once

#include <string>

namespace randomcat::graphics::log {
    enum class LogType : uint8_t { INFO, WARN, ERROR };

    void init();
    void log(std::string const& _message, LogType _type);

    inline void info(std::string const& _message) { log(_message, LogType::INFO); }
    inline void warn(std::string const& _message) { log(_message, LogType::WARN); }
    inline void error(std::string const& _message) { log(_message, LogType::ERROR); }

    inline std::string logTypeString(LogType _logType);
}    // namespace randomcat::graphics::log
