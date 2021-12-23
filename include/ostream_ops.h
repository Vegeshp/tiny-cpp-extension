#pragma once

#ifndef OSTREAM_OPS_H_
#define OSTREAM_OPS_H_

#include <iostream>     // std::ostream
#include <iterator>     // std::cbegin std::cend
#include <tuple>        // std::tuple std::get
#include <type_traits>  // std::true_type std::false_type std::enable_if_t (std::void_t) std::declval
#include <utility>      // std::pair

#if __cplusplus >= 201700L  // since C++17

#define VOID_T std::void_t
#define INLINE inline

template <typename Container>
struct reverse_container {
   public:
    reverse_container(const Container &container) : container(container) {}
    auto cbegin() const { return std::crbegin(container); }
    auto cend() const { return std::crend(container); }
    auto begin() const { return std::rbegin(container); }
    auto end() const { return std::rend(container); }
    auto rbegin() const { return std::rbegin(container); }
    auto rend() const { return std::rend(container); }
    auto crbegin() const { return std::crbegin(container); }
    auto crend() const { return std::crend(container); }

   private:
    const Container &container;
};

template <typename T, size_t N>
struct array_wrapper {
   public:
    array_wrapper(const T (&a)[N]) : array(a) {}
    auto begin() const { return std::begin(array); }
    auto end() const { return std::end(array); }
    auto rbegin() const { return std::rbegin(array); }
    auto rend() const { return std::rend(array); }
    auto cbegin() const { return std::cbegin(array); }
    auto cend() const { return std::cend(array); }
    auto crbegin() const { return std::crbegin(array); }
    auto crend() const { return std::crend(array); }

   private:
    const T (&array)[N];
};

#else  // C++11 or C++14

template <typename...>
using void_t = void;

#define VOID_T void_t
#define INLINE

#endif

#if __cplusplus >= 201400L  // since C++14

#define BEGIN std::cbegin
#define END std::cend

#else  // C++11

#define BEGIN std::begin
#define END std::end

#endif

template <typename T, typename = void>
struct is_iterable
    : public std::false_type {};

template <typename T>
struct is_iterable<T, VOID_T<decltype(BEGIN(std::declval<T>())), decltype(END(std::declval<T>()))>>
    : public std::true_type {};

template <typename T, typename = void>
struct is_ostream_outputable
    : public std::false_type {};

template <typename T>
struct is_ostream_outputable<T,
                             VOID_T<decltype(std::declval<std::ostream &>()
                                             << std::declval<T>())>>
    : public std::true_type {};

#if __cplusplus >= 201400L  // C++14 or later
template <typename T>
INLINE constexpr bool is_iterable_v = is_iterable<T>::value;

template <typename T>
INLINE constexpr bool is_ostream_outputable_v = is_ostream_outputable<T>::value;
#endif

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
    return os << '<' << p.first << ',' << p.second << '>';
}

template <size_t index = 0, typename... Args>
#if __cplusplus >= 201400L  // C++14 or later
std::enable_if_t<index >= sizeof...(Args), std::ostream &>
#else
typename std::enable_if<index >= sizeof...(Args), std::ostream &>::type
#endif
operator<<(std::ostream &os, const std::tuple<Args...> &t) {
    return os << (sizeof...(Args) == 0 ? "<>" : "");
}

template <size_t index = 0, typename... Args>
#if __cplusplus >= 201400L  // C++14 or later
std::enable_if_t<(index < sizeof...(Args)), std::ostream &>
#else
typename std::enable_if<(index < sizeof...(Args)), std::ostream &>::type
#endif
operator<<(std::ostream &os, const std::tuple<Args...> &t) {
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
#if __cplusplus < 201400L  // C++11
typename std::enable_if<is_iterable<Container>::value && !is_ostream_outputable<Container>::value, std::ostream &>::type
#else
std::enable_if_t<is_iterable_v<Container> && !is_ostream_outputable_v<Container>, std::ostream &>
#endif
operator<<(std::ostream &os, const Container &cont) {
    auto it_begin = BEGIN(cont);
    auto it_end = END(cont);
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
