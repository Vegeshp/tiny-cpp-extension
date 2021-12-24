# Tiny C++ Extension

Implemented some convenient utility functions as an extension for C++ standard template library.

## Introduction

### Alias Method

class `AliasMethod` in header `alias_method.h`. Using STL-like implementation, APIs are exactly corresponding to what in `std::discrete_distribution`.

**Support C++11 or above.**

#### Theory in Alias Method

See [Darts, Dice, and Coins: Sampling from a Discrete Distribution
](https://www.keithschwarz.com/darts-dice-coins/) for more information.

### Ostream Operators

For container `cont` of type `Container`, if every element in it can be output via ostream operator, and `cont` itself is *iterable* (for C++17 or above *const-iterable*, which means `std::cbegin(cont)` and `std::cend(cont)` / `std::begin(cont)` and `std::end(cont)` is implemented).

Implemented using *SFINAE*, which means *Substitution Failure Is Not An Error*. By providing multiple possible overload but only one resolution is passed, we can use this overload resolution to avoid multiple function overloads which leads to compilation errors.

Currently, ostream operator for `std::tuple`, `std::vector` and other std-defined containers is valid.

**Support C++11 or above except for array wrapper and reverse container wrapper.**

* Array wrapper and reverse container wrapper require C++17 or above (since class template argument deduction requires C++17 or above)

### Accurate Type Name

Actually, in the beginning template partial specialization is used. However it cannot solve occasion when cv-qualifiers are involved, top-level and low-level of which is vexing. Also, pointer, reference of array is some kind of hard.

Using extension `__PRETTY_FUNCTION__` of gcc and clang is a good idea. There's also a macro `__FUNCSIG__` for MSVC, but I haven't explored it.

Simple character finding is ok. Macro `from_type` and `from_var` are provided.

**Support C++11 or above.**

### Use Elements in Vector to Construct Objects

On some occasions like graphics, a class may have many constructors which requires different number of arguments of the same types when compiling. Thus, it's necessary to avoid messed-up calling code burden. It's a simple idea yet, which requires further work.
