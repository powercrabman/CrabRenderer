#include "CTTI.h"

#include <string>
#include <unordered_map>

namespace crab
{

namespace
{
    std::unordered_map<std::string, TypeIndex>& GetTypeMapper()
    {
        static std::unordered_map<std::string, TypeIndex> s_stringTypeMapper;
        return s_stringTypeMapper;
    }
}

namespace detail
{

    void _RegisterType(TypeName name, const TypeIndex& index)
    {
        GetTypeMapper().emplace(std::string(name), index);
    }

}   // namespace detail

TypeInfo GetTypeInfo(TypeName in_name)
{
    auto it = GetTypeMapper().find(std::string(in_name));
    if (it != GetTypeMapper().end())
    {
        return TypeInfo { in_name, it->second };
    }
    return TypeInfo {};
}   // namespace


}   // namespace crab