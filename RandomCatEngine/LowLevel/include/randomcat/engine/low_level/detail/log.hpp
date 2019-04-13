#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

namespace randomcat::engine::log {
    enum class log_type : uint8_t { INFO, WARN, ERROR };

    namespace log_detail {
        inline std::reference_wrapper<std::ostream> g_logStream = std::reference_wrapper<std::ostream>(std::clog);

        [[nodiscard]] inline std::string_view log_type_header(log_type _logType) noexcept {
            using std::string_view_literals::operator""sv;

            switch (_logType) {
                case log_type::INFO: return "[INFO]"sv;
                case log_type::WARN: return "[WARN]"sv;
                case log_type::ERROR: return "[ERROR]"sv;
            }

            return "[INVALID]"sv;
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

            template<typename T>
            log_impl operator<<(T const& _val) noexcept {
                auto& outStream = g_logStream.get();

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

                    outStream << (isFirst ? "\n" : "") << outString;
                } catch (...) {
                    // Drop error, logging is not vital
                    outStream << "<LOG ERROR>";
                }

                return log_impl(m_logType, cont_type::continued);
            }

            void replace_newlines(std::string& _str, std::string_view _replacement) const noexcept(!"string::replace not noexcept") {
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
            [[nodiscard]] std::string log_header() noexcept(!"Ostreaming not noexcept") {
                std::ostringstream fullMessage{};
                time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto formatTime = *std::localtime(&systemTime);

                fullMessage << "[" << std::put_time(&formatTime, "%Y-%m-%d %H:%M:%S") << "] " << log_type_header(m_logType) << " ";

                return fullMessage.str();
            }

            [[nodiscard]] static std::string_view log_header_cont() noexcept {
                using std::string_view_literals::operator""sv;
                return "\n[CONT] "sv;
            }

            log_type m_logType;
            cont_type m_contType;
        };
    }    // namespace log_detail

    // Client code shall not store any result of this function
    // Guaranteed to be of a type such that:
    // - Any ostreamable type can be logged with the stream insertion operator
    // - Any ostreamable type can be logged with the function call operator
    [[nodiscard]] inline auto log(log_type _type) noexcept { return log_detail::log_impl(std::move(_type)); }

    inline void log(log_type _type, std::string_view _message) noexcept { log(std::move(_type)) << std::move(_message); }

    inline void set_log_output(std::ostream& _stream) noexcept { log_detail::g_logStream = std::ref(_stream); }

    [[nodiscard]] inline std::ostream& raw_log() noexcept { return log_detail::g_logStream; }

    inline auto info = log(log_type::INFO);
    inline auto warn = log(log_type::WARN);
    inline auto error = log(log_type::ERROR);
}    // namespace randomcat::engine::log
