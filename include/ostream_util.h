#pragma once

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

// declaration

template <typename T, typename S>
constexpr std::ostream &
operator<<(std::ostream &os, const std::pair<T, S> &p) noexcept;

template <size_t n = 0, typename... Args>
constexpr typename std::enable_if_t<(n >= sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept;

template <size_t n = 0, typename... Args>
constexpr typename std::enable_if_t<(n < sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept;

// implementation

template <typename T, typename S>
constexpr std::ostream &
operator<<(std::ostream &os, const std::pair<T, S> &p) noexcept {
    return os << '<' << p.first << ',' << p.second << '>';
}

template <size_t n, typename... Args>
constexpr typename std::enable_if_t<(n >= sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept {
    return os;
}

template <size_t n, typename... Args>
constexpr typename std::enable_if_t<(n < sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept {
    return operator<<<n + 1, Args...>(os << (n ? "" : "<") << std::get<n>(t) << (n + 1 == sizeof...(Args) ? '>' : ','), t);
}
