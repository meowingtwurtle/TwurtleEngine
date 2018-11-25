#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <randomcat/engine/detail/log.hpp>

namespace randomcat::engine::log {
    void log(std::string const& _message, LogType _type) noexcept {
        std::ostringstream fullMessage{};
        time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto formatTime = *std::localtime(&systemTime);
        fullMessage << "[" << std::put_time(&formatTime, "%H:%M:%S") << "] [" << log_type_name(_type) << "] " << _message;

        std::cout << fullMessage.str() << std::endl;    // Yes, endl. This flushes.
    }

    std::string log_type_name(LogType _logType) noexcept {
        switch (_logType) {
            case LogType::INFO: return "INFO";
            case LogType::WARN: return "WARN";
            case LogType::ERROR: return "ERROR";
            default:
                return std::string{"Invalid log type: "} + std::to_string(static_cast<std::underlying_type_t<decltype(_logType)>>(_logType));
        }
    }
}    // namespace randomcat::engine::log
