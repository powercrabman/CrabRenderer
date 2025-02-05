#pragma once

#include "TypeIndex.h"
#include "TypeName.h"

namespace crab
{

struct TypeID
{
    TypeName  name  = TypeName();
    TypeIndex index = TypeIndex();

    constexpr bool operator==(const TypeID& other) const { return index == other.index; }
    constexpr bool operator!=(const TypeID& other) const { return index != other.index; }
    constexpr bool operator<(const TypeID& other) const { return index < other.index; }
};

template<class T>
constexpr TypeID GetTypeID()
{
    return TypeID {GetTypeName<T>(), GetTypeIndex<T>() };
}

}   // namespace crab

namespace std
{
template<>
struct hash<crab::TypeID>
{
    hash<crab::TypeIndex> hasher;
    size_t                operator()(const crab::TypeID& id) const { return hasher(id.index); }
};
}   // namespace std
