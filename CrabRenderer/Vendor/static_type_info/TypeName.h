#pragma once

#include <string_view>

namespace crab
{

template<class T>
constexpr std::string_view RawTypeName()
{
#if defined(_MSC_VER)
    return __FUNCSIG__;
#else
    return __PRETTY_FUNCTION__;
#endif
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

using TypeName = std::string_view;

template<class T>
constexpr TypeName GetTypeName()
{
    std::string_view name = RawTypeName<T>();
    return std::string_view(name.data() + probe::prefixLength,
                            name.size() - probe::prefixLength - probe::suffixLength);
}

}   // namespace crab