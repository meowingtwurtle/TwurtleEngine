#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

namespace randomcat::engine::log {
    enum class log_type : uint8_t { INFO, WARN, ERROR };

    namespace detail {
        inline std::atomic<std::reference_wrapper<std::ostream>> g_logStream = std::atomic<std::reference_wrapper<std::ostream>>(std::clog);
        inline std::atomic_flag g_isNotFirstLog = ATOMIC_FLAG_INIT;

        inline std::string log_type_header(log_type _logType) noexcept {
            switch (_logType) {
                case log_type::INFO: return "[INFO]";
                case log_type::WARN: return "[WARN]";
                case log_type::ERROR: return "[ERROR]";
            }

            return "[INVALID]";
        }

        // Client code shall not use this type except as the result of library-provided
        // functions, even if it is the result of a decltype or auto variable. Client
        // code shall not store any instance of this type.
        class log_impl {
        public:
            enum class cont_type { first, continued };

            explicit log_impl(log_type _logType, cont_type _contType = cont_type::first) noexcept
            : m_logType(std::move(_logType)), m_contType(std::move(_contType)) {}

            log_impl(log_impl const&) = delete;
            log_impl(log_impl&&) = delete;
            log_impl operator=(log_impl const&) = delete;
            log_impl operator=(log_impl&&) = delete;

            template<typename T>
            log_impl operator<<(T const& _val) noexcept {
                auto& outStream = g_logStream.load(std::memory_order_acquire).get();

                try {
                    std::ostringstream tempOut;

                    bool const isFirst = (m_contType == cont_type::first);

                    if (isFirst)
                        tempOut << log_header();
                    else {
                    }; /* continued, don't output header */

                    tempOut << _val;

                    std::string outString = std::move(tempOut).str();

                    replace_newlines(outString, log_header_cont());

                    outStream << ((isFirst && g_isNotFirstLog.test_and_set()) ? "\n" : "") << outString;
                } catch (...) {
                    // Drop error, logging is not vital
                    outStream << "<LOG ERROR>";
                }

                return log_impl(m_logType, cont_type::continued);
            }

            void replace_newlines(std::string& _str, std::string const& _replacement) const noexcept(false) {
                auto newLinePos = _str.find('\n');

                while (newLinePos != std::string::npos) {
                    _str.replace(newLinePos, 1, _replacement);
                    newLinePos = _str.find('\n', newLinePos + _replacement.length());
                }
            }

            template<typename T>
            void operator()(T const& _val) noexcept {
                (*this) << _val;
            }

        private:
            std::string log_header() noexcept(false) {
                std::ostringstream fullMessage{};
                time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto formatTime = *std::localtime(&systemTime);

                fullMessage << "[" << std::put_time(&formatTime, "%Y-%m-%d %H:%M:%S") << "] " << log_type_header(m_logType) << " ";

                return fullMessage.str();
            }

            static std::string log_header_cont() noexcept(false) { return "\n[CONT] "; }

            log_type m_logType;
            cont_type m_contType;
        };
    }    // namespace detail

    // Client code shall not store any result of this function
    // Guaranteed to be of a type such that:
    // - Any ostreamable type can be logged with the stream insertion operator
    // - Any ostreamable type can be logged with the function call operator
    inline auto log(log_type _type) noexcept { return detail::log_impl(std::move(_type)); }

    inline void log(log_type _type, std::string_view _message) noexcept { log(std::move(_type)) << std::move(_message); }

    inline void set_log_stream(std::ostream& _stream) { detail::g_logStream.store(std::ref(_stream), std::memory_order_release); }

    inline auto info = log(log_type::INFO);
    inline auto warn = log(log_type::WARN);
    inline auto error = log(log_type::ERROR);
}    // namespace randomcat::engine::log
