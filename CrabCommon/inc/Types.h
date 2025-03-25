#pragma once

#include <memory>

namespace crab
{

//===================================================
// Data Types
//===================================================

using int8  = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using wchar = wchar_t;
using Index = uint32;

//===================================================
// Smart Pointer
//===================================================

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
inline Scope<T> CreateScope(Args&&... args)
{
    return Scope<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
inline Ref<T> CreateRef(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

//===================================================
// Min Max Value
//===================================================

constexpr uint32 UINT32_MAX_VALUE = std::numeric_limits<uint32>::max();
constexpr uint32 UINT32_MIN_VALUE = std::numeric_limits<uint32>::min();

constexpr int32 INT32_MAX_VALUE = std::numeric_limits<int32>::max();
constexpr int32 INT32_MIN_VALUE = std::numeric_limits<int32>::min();

constexpr float FLOAT_MAX_VALUE = std::numeric_limits<float>::max();
constexpr float FLOAT_MIN_VALUE = std::numeric_limits<float>::min();

}   // namespace crab