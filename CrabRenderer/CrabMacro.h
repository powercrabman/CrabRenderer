#pragma once

// - Assert
#if defined(_DEBUG) | 1
#    define CRAB_ASSERT(x, hint)                                           \
        do                                                                 \
        {                                                                  \
            if (!(x))                                                      \
            {                                                              \
                std::string message = fmt::format("**********\n"           \
                                                  "Crab assert fail: {}\n" \
                                                  "**********\n",          \
                                                  hint);                   \
                Log::Error(message.c_str());                               \
                OutputDebugStringA(message.c_str());                       \
                __debugbreak();                                            \
            }                                                              \
        } while (0)

#    define CRAB_ASSERT_V(x, hint_format, ...)                                     \
        do                                                                         \
        {                                                                          \
            if (!(x))                                                              \
            {                                                                      \
                std::string message = fmt::format("**********\n"                   \
                                                  "Crab assert fail: " hint_format \
                                                  "\n**********\n",                \
                                                  __VA_ARGS__);                    \
                Log::Error(message.c_str());                                       \
                OutputDebugStringA(message.c_str());                               \
                __debugbreak();                                                    \
            }                                                                      \
        } while (0)

#    define CRAB_DEBUG_BREAK(hint)               CRAB_ASSERT(false, hint)
#    define CRAB_DEBUG_BREAK_V(hint_format, ...) CRAB_ASSERT_V(false, hint_format, __VA_ARGS__)

#    define D11_ASSERT(hr, hint)                                            \
        do                                                                  \
        {                                                                   \
            if (FAILED(hr))                                                 \
            {                                                               \
                std::string hrMessage = TranslateHRESULT(hr);               \
                std::string message   = fmt::format("**********\n"          \
                                                    "D11 assert fail: {}\n" \
                                                    "HRESULT: {}"           \
                                                    "**********\n",         \
                                                  hint,                   \
                                                  hrMessage.c_str());     \
                Log::Error(message.c_str());                                \
                OutputDebugStringA(message.c_str());                        \
                __debugbreak();                                             \
            }                                                               \
        } while (0)

#    define D11_ASSERT_V(hr, hint_format, ...)                                                  \
        do                                                                               \
        {                                                                                \
            if (FAILED(hr))                                                              \
            {                                                                            \
                std::string hrMessage = TranslateHRESULT(hr);                            \
                std::string message   = fmt::format("**********\n"                       \
                                                    "D11 assert fail: " hint_format "\n" \
                                                    "HRESULT: {}"                        \
                                                    "**********\n",                      \
                                                  __VA_ARGS__,                         \
                                                  hrMessage.c_str());                  \
                Log::Error(message.c_str());                                             \
                OutputDebugStringA(message.c_str());                                     \
                __debugbreak();                                                          \
            }                                                                            \
        } while (0)

#else
#    define CRAB_ASSERT(x, hint)                 ((void)0)
#    define CRAB_ASSERT_V(x, hint_format, ...)   ((void)0)
#    define CRAB_DEBUG_BREAK(hint)               ((void)0)
#    define CRAB_DEBUG_BREAK_V(hint_format, ...) ((void)0)
#    define D11_ASSERT(hr, hint)                 ((void)0)
#    define D11_ASSERT_V(hr, hint, ...)          ((void)0)
#endif

// - Template Meta Programming
#define IS_SAME(Type1, Type2)     std::is_same<Type1, Type2>::value
#define IS_BASE_OF(Base, Derived) std::is_base_of<Base, Derived>::value

// - Bit
#define BIT(x) (1 << x)
#define BIT_AND(x, y) ((x) & (y))
#define BIT_OR(x, y)  ((x) | (y))
#define BIT_XOR(x, y) ((x) ^ (y))
#define BIT_NOT(x)    (~(x))

// - Rendering
#define cbuffer       struct alignas(16)