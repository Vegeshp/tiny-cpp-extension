# Tiny C++ Extension

Implemented some convenient utility functions as an extension for C++ standard template library.

## Introduction

### Alias Method

class `AliasMethod` in header `alias_method.h`. Using STL-like implementation, APIs are exactly corresponding to what in `std::discrete_distribution`.

#### Theory in Alias Method

See [Darts, Dice, and Coins: Sampling from a Discrete Distribution
](https://www.keithschwarz.com/darts-dice-coins/) for more information.

### Ostream Operators

For container `cont` of type `Container`, if every element in it can be output via ostream operator, and `cont` itself is *const-iterable* (which means `std::cbegin(cont)` and `std::cend(cont)` is implemented).

Implemented using *SFINAE*, which means *Substitution Failure Is Not An Error*. By providing multiple possible overload but only one resolution is passed, we can use this overload resolution to execute.

Currently, ostream operator for `std::tuple`, `std::vector` and other std-defined containers is valid.

### Accurate Type Name

Actually, in the beginning template partial specialization is used. However it cannot solve occasion when cv-qualifiers are involved, top-level and low-level of which is vexing. Also, pointer, reference of array is some kind of hard.

Using extension `__PRETTY_FUNCTION__` of gcc and clang is a good idea. There's also a macro `__MSCSIG__` for MSVC, but I haven't explored it.

Simple character finding is ok. Macro `from_type` and `from_var` are provided.
