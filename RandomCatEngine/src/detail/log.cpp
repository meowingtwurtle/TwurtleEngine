#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <randomcat/engine/detail/log.h>

namespace randomcat::engine::log {
    void log(std::string const& _message, LogType _type) {
        std::ostringstream fullMessage{};
        time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto formatTime = *std::localtime(&systemTime);
        fullMessage << "[" << std::put_time(&formatTime, "%H:%M:%S") << "] [" << logTypeString(_type) << "] " << _message;

        std::cout << fullMessage.str() << std::endl;    // Yes, endl. This flushes.
    }

    std::string logTypeString(LogType _logType) {
        switch (_logType) {
            case LogType::INFO: return "INFO";
            case LogType::WARN: return "WARN";
            case LogType::ERROR: return "ERROR";
            default:
                throw std::logic_error{std::string{"Invalid log type: "}
                                       + std::to_string(static_cast<std::underlying_type_t<decltype(_logType)>>(_logType))};
        }
    }
}    // namespace randomcat::engine::log
