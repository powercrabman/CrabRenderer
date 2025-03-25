#pragma once

enum class eLogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

namespace crab
{

class Log
{
public:
    // - Setting
    static void SetLogLevel(eLogLevel in_level);

    // - Logging
    static void Trace(const char* in_message);
    static void Debug(const char* in_message);
    static void Info(const char* in_message);
    static void Warn(const char* in_message);
    static void Error(const char* in_message);
    static void Critical(const char* in_message);

private:
    struct Impl;
    static Scope<Impl> m_impl;
};

namespace log
{
    inline void Trace(const char* in_message) { Log::Trace(in_message); }
    inline void Debug(const char* in_message) { Log::Debug(in_message); }
    inline void Info(const char* in_message) { Log::Info(in_message); }
    inline void Warn(const char* in_message) { Log::Warn(in_message); }
    inline void Error(const char* in_message) { Log::Error(in_message); }
    inline void Critical(const char* in_message) { Log::Critical(in_message); }

    template<typename... Args>
    inline void Info(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Info(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

    template<typename... Args>
    inline void Debug(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Debug(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

    template<typename... Args>
    inline void Trace(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Trace(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

    template<typename... Args>
    inline void Warn(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Warn(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

    template<typename... Args>
    inline void Error(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Error(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

    template<typename... Args>
    inline void Critical(fmt::format_string<Args...> in_format, Args&&... in_args)
    {
        Log::Critical(fmt::format(in_format, std::forward<Args>(in_args)...).c_str());
    }

}   // namespace log

}   // namespace crab
