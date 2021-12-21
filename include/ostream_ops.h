#pragma once

#ifndef OSTREAM_OPS_H_
#define OSTREAM_OPS_H_

#include <iostream>     // std::ostream
#include <tuple>        // tuple
#include <type_traits>  // std::enable_if_t
#include <utility>      // std::pair

template <typename T, typename = void>
struct is_const_iterable
    : public std::false_type {};

template <typename T>
struct is_const_iterable<T, std::void_t<decltype(std::cbegin(std::declval<T>()))>>
    : public std::true_type {};

template <typename T, typename = void>
struct is_ostream_outputable
    : public std::false_type {};

template <typename T>
struct is_ostream_outputable<T,
                             std::void_t<decltype(std::declval<std::ostream &>()
                                                  << std::declval<T>())>>
    : public std::true_type {};

template <typename T>
inline constexpr bool is_const_iterable_v = is_const_iterable<T>::value;

template <typename T>
inline constexpr bool is_ostream_outputable_v = is_ostream_outputable<T>::value;

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
    return os << '<' << p.first << ',' << p.second << '>';
}

template <size_t index = 0, typename... Args>
auto operator<<(std::ostream &os, const std::tuple<Args...> &t)
    -> std::enable_if_t<index >= sizeof...(Args), std::ostream &> {
    return os << (sizeof...(Args) == 0 ? "<>" : "");
}

template <size_t index = 0, typename... Args>
auto operator<<(std::ostream &os, const std::tuple<Args...> &t)
    -> std::enable_if_t<(index < sizeof...(Args)), std::ostream &> {
    return operator<<<index + 1, Args...>(os
                                              // if it's not the first, output blank as left border
                                              << (index != 0 ? "" : "<")
                                              // concrete content
                                              << std::get<index>(t)
                                              // if it's the last, output > as right border, else output broker
                                              << (index + 1 == sizeof...(Args) ? '>' : ','),
                                          t);
}

template <typename Container>
auto operator<<(std::ostream &os, const Container &cont)
    -> std::enable_if_t<is_const_iterable_v<Container> &&
                            !is_ostream_outputable_v<Container>,
                        std::ostream &> {
    auto it_begin = std::cbegin(cont);
    auto it_end = std::cend(cont);
    os << '{';
    while (it_begin != it_end) {
        os << *it_begin;
        ++it_begin;
        if (it_begin != it_end) {
            os << ", ";
        }
    }
    return os << '}';
}

#endif  // OSTREAM_OPS_H_
