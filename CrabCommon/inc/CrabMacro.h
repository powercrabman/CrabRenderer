#pragma once

// Template Meta Programming
#define IS_SAME(Type1, Type2)     std::is_same<Type1, Type2>::value
#define IS_BASE_OF(Base, Derived) std::is_base_of<Base, Derived>::value

// Bit
#define BIT(x)        (1 << x)
#define BIT_AND(x, y) ((x) & (y))
#define BIT_OR(x, y)  ((x) | (y))
#define BIT_XOR(x, y) ((x) ^ (y))
#define BIT_NOT(x)    (~(x))

// Attributes
#define NODISCARD [[nodiscard]]
#define OPTIONAL
