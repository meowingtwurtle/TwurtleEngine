#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <twurtle/log.h>

namespace randomcat::graphics::log {
    void log(std::string const& _message, LogType _type) {
        std::ostringstream fullMessage{};
        time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto formatTime = *std::localtime(&systemTime);
        fullMessage << "[" << std::put_time(&formatTime, "%H:%M:%S") << "] [" << logTypeString(_type) << "] " << _message;

        std::cout << fullMessage.str() << std::endl;    // Yes, endl. This flushes.
    }

    struct bad_log_type {
        explicit bad_log_type(LogType _logType) : logType(_logType) {}

        std::string what() { return std::string{"Bad log type: "} + std::to_string(static_cast<std::underlying_type_t<LogType>>(logType)); }

        LogType logType;
    };

    std::string logTypeString(LogType _logType) {
        switch (_logType) {
            case LogType::INFO: return "INFO";
            case LogType::WARN: return "WARN";
            case LogType::ERROR: return "ERROR";
            default: throw bad_log_type{_logType};
        }
    }
}    // namespace randomcat::graphics::log
