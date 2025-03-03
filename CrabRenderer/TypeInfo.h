﻿#pragma once

#include <functional> 
#include <string>
#include <string_view>

#ifndef STATIC_TYPE_INFO_USE_MEMBER_POINTER
#    if defined(__clang__) || defined(_MSC_VER) || defined(EMSCRIPTEN)
#        define STATIC_TYPE_INFO_USE_MEMBER_POINTER true
#    else
#        define STATIC_TYPE_INFO_USE_MEMBER_POINTER false
#    endif
#endif

#if STATIC_TYPE_INFO_USE_MEMBER_POINTER

namespace crab
{

using TypeName = std::string_view;

//===================================================
// TypeIndex
//===================================================

namespace detail
{

    template<typename T>
    struct IDGenerator
    {
        static const void* id;
    };

    template<typename T>
    const void* IDGenerator<T>::id = nullptr;

}   // namespace detail

//===================================================
// TypeIndex
//===================================================

using TypeIndex = decltype(&detail::IDGenerator<void>::id);

//===================================================
// Compile Time Hash
//===================================================

// FNV-1a hash function
constexpr uint32_t VAL_32_CONST   = 0x811c9dc5;
constexpr uint32_t PRIME_32_CONST = 0x1000193;
constexpr uint64_t VAL_64_CONST   = 0xcbf29ce484222325;
constexpr uint64_t PRIME_64_CONST = 0x100000001b3;

inline constexpr uint32_t Hash32_FNV1A_const(std::string_view str,
                                             const uint32_t   value = VAL_32_CONST) noexcept
{
    return (str.size() == 0) ? value : Hash32_FNV1A_const(std::string_view(str.data() + 1, str.size() - 1), (value ^ uint32_t(str[0])) * PRIME_32_CONST);
}

inline constexpr uint64_t Hash64_FNV1A_const(std::string_view str,
                                             const uint64_t   value = VAL_64_CONST) noexcept
{
    return (str.size() == 0) ? value : Hash64_FNV1A_const(std::string_view(str.data() + 1, str.size() - 1), (value ^ uint64_t(str[0])) * PRIME_64_CONST);
}

//===================================================
// TypeName
//===================================================

template<typename T>
constexpr std::string_view RawTypeName()
{
#    if defined(_MSC_VER)
    return __FUNCSIG__;
#    else
    return __PRETTY_FUNCTION__;
#    endif
}

namespace probe
{
    constexpr auto   probeRawTypeName   = RawTypeName<double>();
    constexpr size_t probeRawTypeLength = std::string_view("double").size();
    constexpr size_t prefixLength       = probeRawTypeName.find("double");
    static_assert(prefixLength != std::string_view::npos,
                  "cannot extract typename from function signature");
    constexpr size_t suffixLength = probeRawTypeName.size() - prefixLength - probeRawTypeLength;
}   // namespace probe

namespace detail
{

    template<typename T>
    constexpr TypeName _GetTypeName()
    {
        std::string_view name = RawTypeName<T>();
        return std::string_view(name.data() + probe::prefixLength,
                                name.size() - probe::prefixLength - probe::suffixLength);
    }

}   // namespace detail

//===================================================
// TypeIndex
//===================================================

namespace detail
{

    template<class T>
    constexpr TypeIndex _GetTypeIndex()
    {
        return &IDGenerator<T>::id;
    }

}   // namespace detail

//===================================================
// TypeInfo
//===================================================

struct TypeInfo
{
    TypeName  name  = TypeName();
    TypeIndex index = TypeIndex();

    constexpr bool operator==(const TypeInfo& other) const { return index == other.index; }
    constexpr bool operator!=(const TypeInfo& other) const { return index != other.index; }
    constexpr bool operator<(const TypeInfo& other) const { return index < other.index; }

    template<typename Ty>
    static TypeInfo Get()
    {
        return { detail::_GetTypeName<Ty>(), detail::_GetTypeIndex<Ty>() };
    }
};


inline std::string ExtractStringInTypeName(TypeName in_name)
{
    size_t idx = in_name.find_first_of(' ');
    return std::string(in_name.substr(idx + 1));
}

#include <entt/core/hashed_string.hpp>
inline constexpr uint32_t HashString(std::string_view in_string)
{
    return entt::hashed_string::value(in_string.data());
}


}   // namespace crab

namespace std
{

template<>
struct hash<crab::TypeIndex>
{
    size_t operator()(const crab::TypeIndex& index) const noexcept
    {
        // TypeIndex가 const void** 타입이므로, 이를 const void*로 변환하여 해시
        return std::hash<const void*>()(reinterpret_cast<const void*>(index));
    }
};


template<>
struct hash<crab::TypeInfo>
{
    std::hash<crab::TypeIndex> hasher;
    size_t                     operator()(const crab::TypeInfo& id) const { return hasher(id.index); }
};

#else

using TypeIndex = uint64_t;

template<typename T>
constexpr TypeIndex getTypeIndex()
{
    return Hash64_FNV1A_const(getTypeName<T>());
}

#endif

}   // namespace std
