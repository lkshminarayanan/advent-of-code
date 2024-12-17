#pragma once

#include <format>
#include <iostream>

class Logger {

  public:
    enum class LogLevel { DEBUG, INFO, WARN, ERROR };

  private:
    std::string _logger_name;
    LogLevel _log_level;

    std::string log_level_to_string(LogLevel level) {
        switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO ";
        case LogLevel::WARN:
            return "WARN ";
        case LogLevel::ERROR:
            return "ERROR";
        }
    }

    template <typename... Args>
    void log(const std::format_string<Args...>& fmt, LogLevel level, Args&&... args) {
        if (_log_level <= level) {
            std::cout << std::format("{} [{}] : {}", log_level_to_string(level), _logger_name,
                                     std::format(fmt, std::forward<Args>(args)...))
                      << std::endl;
        }
    }

  public:
    Logger(const std::string& logger_name, LogLevel log_level = LogLevel::INFO)
        : _logger_name(logger_name), _log_level(log_level) {}

    template <typename... Args> void debug(const std::format_string<Args...>& fmt, Args&&... args) {
        log(fmt, LogLevel::DEBUG, std::forward<Args>(args)...);
    }

    template <typename... Args> void info(const std::format_string<Args...>& fmt, Args&&... args) {
        log(fmt, LogLevel::INFO, std::forward<Args>(args)...);
    }

    template <typename... Args> void warn(const std::format_string<Args...>& fmt, Args&&... args) {
        log(fmt, LogLevel::WARN, std::forward<Args>(args)...);
    }

    template <typename... Args> void error(const std::format_string<Args...>& fmt, Args&&... args) {
        log(fmt, LogLevel::ERROR, std::forward<Args>(args)...);
    }
};
