#pragma once
#include <spdlog/spdlog.h>

namespace crab
{

class Log
{
public:
    // - Base
    static void Init();
    static void Shutdown();

    // - Setting
    static void SetLogLevel(eLogLevel in_level);

    // - Logging
    static void Trace(const char* in_message);
    static void Debug(const char* in_message);
    static void Info(const char* in_message);
    static void Warn(const char* in_message);
    static void Error(const char* in_message);
    static void Critical(const char* in_message);

    template<typename... Args>
    static void Trace(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::trace(fmt, std::forward<Args>(in_args)...);
    }

    template<typename... Args>
    static void Debug(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::debug(fmt, std::forward<Args>(in_args)...);
    }

    template<typename... Args>
    static void Info(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::info(fmt, std::forward<Args>(in_args)...);
    }

    template<typename... Args>
    static void Warn(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::warn(fmt, std::forward<Args>(in_args)...);
    }

    template<typename... Args>
    static void Error(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::error(fmt, std::forward<Args>(in_args)...);
    }

    template<typename... Args>
    static void Critical(fmt::format_string<Args...> fmt, Args&&... in_args)
    {
        spdlog::critical(fmt, std::forward<Args>(in_args)...);
    }
};

}   // namespace crab
