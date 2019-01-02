#pragma once

#include <chrono>
#include <functional>
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

    namespace detail {
        inline std::reference_wrapper<std::ostream> g_logStream = std::clog;

        class log_impl {
        public:
            enum class cont_type { first, continued };

            explicit log_impl(log_type _logType, cont_type _contType = cont_type::first)
            : m_logType(std::move(_logType)), m_contType(std::move(_contType)) {}

            log_impl(log_impl const&) = delete;
            log_impl(log_impl&&) = delete;
            log_impl operator=(log_impl const&) = delete;
            log_impl operator=(log_impl&&) = delete;

            template<typename T>
            log_impl operator<<(T const& _val) noexcept(false) {
                std::ostringstream tempOut;

                bool const isFirst = (m_contType == cont_type::first);

                if (isFirst)
                    tempOut << log_header();
                else {
                }; /* continued, don't output header */

                tempOut << _val;

                std::string outString = std::move(tempOut).str();

                replace_newlines(outString, log_header_cont());

                (g_logStream.get()) << (isFirst ? "\n" : "") << outString;

                return log_impl(m_logType, cont_type::continued);
            }

            void replace_newlines(std::string& _str, std::string const& _replacement) const {
                auto newLinePos = _str.find('\n');

                while (newLinePos != std::string::npos) {
                    _str.replace(newLinePos, 1, _replacement);
                    newLinePos = _str.find('\n', newLinePos + _replacement.length());
                }
            }

            template<typename T>
            void operator()(T const& _val) noexcept(false) {
                (*this) << _val;
            }

        private:
            std::string log_header() noexcept(false) {
                std::ostringstream fullMessage{};
                time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto formatTime = *std::localtime(&systemTime);
                fullMessage << "[" << std::put_time(&formatTime, "%Y-%m-%d %H:%M:%S") << "] [" << log_type_name(m_logType) << "] ";

                return fullMessage.str();
            }

            static std::string log_header_cont() noexcept(false) { return "\n[CONT] "; }

            log_type m_logType;
            cont_type m_contType;
        };
    }    // namespace detail

    inline detail::log_impl log(log_type _type) noexcept { return detail::log_impl(std::move(_type)); }

    inline auto info = log(log_type::INFO);
    inline auto warn = log(log_type::WARN);
    inline auto error = log(log_type::ERROR);
}    // namespace randomcat::engine::log
