#pragma once

#ifndef FILT
#include <cxxabi.h>
#endif

#include <iostream>
#include <string>
#include <type_traits>

int count = 0;

template <typename T>
struct RealType {
    static std::string name() {
        using type = std::remove_cv_t<T>;
#ifndef FILT
        std::string type_str = abi::__cxa_demangle(typeid(type).name(), nullptr,
                                                   nullptr, nullptr),
#else
        std::string type_str = typeid(type).name(),
#endif
                    prefix;
        if (std::is_const<T>{}()) {
            prefix.append("const ");
        }
        if (std::is_volatile<T>{}()) {
            prefix.append("volatile ");
        }
        return prefix.append(type_str);
    }
};

template <typename T>
std::string get_name() {
    return RealType<T>::name();
}

namespace __detail {
template <typename T>
std::string three_parts(std::string a, std::string b) {
    return get_name<T>().append(a).append(b);
}
template <typename T>
std::string append(const std::string& q) {
    return get_name<T>().append(q);
}
};  // namespace __detail

template <typename T>
struct RealType<T*> : public RealType<T> {
    static std::string name() { return __detail::append<T>("*"); }
};

template <typename T>
struct RealType<T* const> : public RealType<T> {
    static std::string name() { return __detail::append<T>("* const"); }
};

template <typename T>
struct RealType<T* volatile> : public RealType<T> {
    static std::string name() { return __detail::append<T>("* volatile"); }
};

template <typename T>
struct RealType<T* const volatile> : public RealType<T> {
    static std::string name() {
        return __detail::append<T>("* const volatile");
    }
};

template <typename T>
struct RealType<T&> : public RealType<T> {
    static std::string name() { return __detail::append<T>("&"); }
};

template <typename T>
struct RealType<T&&> : public RealType<T> {
    static std::string name() { return __detail::append<T>("&&"); }
};

namespace __detail {
template <typename T>
std::string to_array(const std::string& ops, size_t size) {
    return __detail::three_parts<T>(" (" + ops + ") [",
                                    std::to_string(size).append("]"));
}
template <typename T = void>
std::string iterate_params() {
    return (std::is_same<T, void>{}() ? "" : get_name<T>()).append(")");
}

template <typename T, typename... Args>
std::enable_if_t<sizeof...(Args) >= 1, std::string> iterate_params() {
    return get_name<T>().append(", ").append(iterate_params<Args...>());
}

template <typename RetType, typename... Args>
std::string to_function(const std::string& ops) {
    return __detail::three_parts<RetType>(" (" + ops + ") (",
                                          __detail::iterate_params<Args...>());
}
}  // namespace __detail

template <typename T, size_t _Size>
struct RealType<T (*)[_Size]> : public RealType<T> {
    static std::string name() { return __detail::to_array<T>("*", _Size); }
};
template <typename T, size_t _Size>
struct RealType<T (&)[_Size]> : public RealType<T> {
    static std::string name() { return __detail::to_array<T>("&", _Size); }
};

template <typename T, size_t _Size>
struct RealType<T (*&)[_Size]> : public RealType<T> {
    static std::string name() { return __detail::to_array<T>("*&", _Size); }
};
template <typename T, size_t _Size>
struct RealType<T(&&)[_Size]> : public RealType<T> {
    static std::string name() { return __detail::to_array<T>("&&", _Size); }
};

template <typename T, size_t _Size>
struct RealType<T (*&&)[_Size]> : public RealType<T> {
    static std::string name() { return __detail::to_array<T>("*&&", _Size); }
};

template <typename RetType, typename... Args>
struct RealType<RetType (*)(Args...)> {
    static std::string name() {
        return __detail::to_function<RetType, Args...>("*");
    }
};

template <typename RetType, typename... Args>
struct RealType<RetType (&)(Args...)> {
    static std::string name() {
        return __detail::to_function<RetType, Args...>("&");
    }
};

template <typename RetType, typename... Args>
struct RealType<RetType(&&)(Args...)> {
    static std::string name() {
        return __detail::to_function<RetType, Args...>("&&");
    }
};

template <typename RetType, typename... Args>
struct RealType<RetType (*&)(Args...)> {
    static std::string name() {
        return __detail::to_function<RetType, Args...>("*&");
    }
};

template <typename RetType, typename... Args>
struct RealType<RetType (*&&)(Args...)> {
    static std::string name() {
        return __detail::to_function<RetType, Args...>("*&&");
    }
};
