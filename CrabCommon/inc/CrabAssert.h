#pragma once

#ifdef _DEBUG
#    define ASSERT(condition, in_message)                                                 \
        do                                                                                \
        {                                                                                 \
            if (!(condition))                                                             \
            {                                                                             \
                log::Error("============== Assertion Failed ==============");             \
                log::Error(in_message);                                                   \
                log::Error("==============================================");             \
                                                                                          \
                OutputDebugStringA("============== Assertion Failed ==============\n");   \
                OutputDebugStringA(in_message);                                           \
                OutputDebugStringA("==============================================\n"); \
                __debugbreak();                                                           \
            }                                                                             \
        } while (0)

#else
#    define ASSERT(condition, in_message)                                     \
        do                                                                    \
        {                                                                     \
            if (!(condition))                                                 \
            {                                                                 \
                log::Error("============== Assertion Failed =============="); \
                log::Error(in_message);                                       \
                log::Error("=============================================="); \
                __debugbreak();                                               \
            }                                                                 \
        } while (0)

#endif

#define DEBUG_BREAK(message) ASSERT(false, message)