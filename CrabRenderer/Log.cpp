#include "CrabPch.h"

#include "Log.h"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace crab
{

void Log::Init(const LogSetting& in_setting)
{
    // - Create log directory
    std::filesystem::create_directories(in_setting.logDirectory);

    // - Create console sink
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // - Create file sink
    const auto            now         = std::chrono::system_clock::now();
    const auto            timestamp   = std::format("{:%Y-%m-%d %H-%M}", now);
    std::filesystem::path logFilePath = in_setting.logDirectory / timestamp / ".txt";
    auto                  fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);

    // - Sinks
    std::vector<spdlog::sink_ptr> sinks { consoleSink, fileSink };

    // - Async logger
    size_t threadPoolSize = 8192;
    size_t threadCount    = 1;
    spdlog::init_thread_pool(threadPoolSize, threadCount);

    auto logger = std::make_shared<spdlog::async_logger>(
        "multi_sink",
        sinks.begin(),
        sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    spdlog::set_default_logger(logger);

    // - Setting
    spdlog::set_pattern(in_setting.logPattern);
    spdlog::set_level(static_cast<spdlog::level::level_enum>(in_setting.logLevel));

    // - Done
    spdlog::info("Log initialized");
}

void Log::Shutdown()
{
    spdlog::shutdown();
}

void Log::SetLogLevel(eLogLevel in_level)
{
    spdlog::set_level(static_cast<spdlog::level::level_enum>(in_level));
}

void Log::Debug(const char* in_message)
{
    spdlog::debug(in_message);
}

void Log::Info(const char* in_message)
{
    spdlog::info(in_message);
}

void Log::Warn(const char* in_message)
{
    spdlog::warn(in_message);
}

void Log::Error(const char* in_message)
{
    spdlog::error(in_message);
}

void Log::Critical(const char* in_message)
{
    spdlog::critical(in_message);
}

void Log::Trace(const char* in_message)
{
    spdlog::trace(in_message);
}

}   // namespace crab