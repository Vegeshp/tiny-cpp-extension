#pragma once

#ifndef TYPE_NAME_H_
#define TYPE_NAME_H_

#include <string>

template <typename T>
struct TypeName {
    static std::string name() {
#if defined(__clang__)
        std::string res = __PRETTY_FUNCTION__;
        size_t index = res.find(">::name()");
        return res.substr(28, index - 28);
#elif defined(__GNUC__) || defined(__GNUG__)
        std::string res = __PRETTY_FUNCTION__;
        size_t index = res.find(';');
        return res.substr(49, index - 49);
#elif defined(_MSC_VER)
        static_assert(false, __FUNCSIG__ "Not implemented");
#else
        static_assert(false, "Unknown compiler")
#endif
        return res;
    }
};

#define from_type(x) TypeName<x>::name()
#define from_var(x) TypeName<decltype(x)>::name()

#endif  // TYPE_NAME_H_
