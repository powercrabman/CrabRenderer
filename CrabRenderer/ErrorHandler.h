#pragma once
#include "Log.h"

namespace crab
{

namespace d3d
{
    std::string TranslateHRESULT(HRESULT hr);

    // Check Succeeded HRESULT
    bool CheckOK(HRESULT hr, std::string_view in_hintIfFailed);
    bool CheckOK(HRESULT hr);

    // Check Failed HRESULT
    bool CheckFail(HRESULT hr, std::string_view in_hintIfFailed);
    bool CheckFail(HRESULT hr);

}   // namespace d3d

#ifdef _DEBUG
#    define ASSERT(condition, in_message)                      \
        do                                                     \
        {                                                      \
            if (!(condition))                                    \
            {                                                  \
                std::string message = fmt::format(R"(         \
        \n============================\n                      \
        Assertion Failed: {0}                             \
        \n============================\n                      \
        )",                                                    \
                                                  in_message); \
                                                               \
                Log::Error(message.c_str());                   \
                                                               \
                OutputDebugStringA(message.c_str());           \
                __debugbreak();                                \
            }                                                  \
        } while (0)
#else
#    define ASSERT(condition, in_message)                     \
        do                                                     \
        {                                                      \
            if (!condition)                                    \
            {                                                  \
                std::string message = fmt::format(R"(         \
        \n============================\n                      \
        Assertion Failed: {0}                             \
        \n============================\n                      \
        )",                                                    \
                                                  in_message); \
                                                               \
                Log::Error(message.c_str());                   \
            }                                                  \
        } while (0)
#endif

#define DEBUG_BREAK(message) ASSERT(false, message)

}   // namespace crab