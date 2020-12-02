#pragma once

#include <iostream>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

/**
 * output pair<T, S>, tuple<A, B, ...>, container that supports iterator begin, end.
 */

// declaration
template <typename T>
struct is_iterable;

template <typename T, typename S>
constexpr std::ostream &
operator<<(std::ostream &os, const std::pair<T, S> &p) noexcept;

template <size_t n = 0, typename... Args>
constexpr typename std::enable_if_t<(n >= sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept;

template <size_t n = 0, typename... Args>
constexpr typename std::enable_if_t<(n < sizeof...(Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<Args...> &t) noexcept;

template <typename Container>
constexpr typename std::enable_if_t<is_iterable<Container>{}(), std::ostream &>
operator<<(std::ostream &os, const Container &v) noexcept;

template <typename Container>
constexpr typename std::enable_if_t<is_iterable<Container>{}(), std::ostream &>
ostream_container_impl(std::ostream &os, const Container &v, typename Container::const_iterator cit) noexcept;

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

template <typename T>
struct is_iterable {
   public:
    constexpr bool operator()() noexcept { return value; }

   private:
    template <typename Tp>
    constexpr static std::true_type exist_type(typename Tp::const_iterator *, decltype(&Tp::cbegin), decltype(&Tp::cend)) noexcept;
    template <typename Tp>
    constexpr static std::false_type exist_type(...) noexcept;
    using exist = decltype(exist_type<T>(nullptr, nullptr, nullptr));
    constexpr static bool value = std::is_same<exist, std::true_type>{}() && !std::is_same<T, std::string>{}();
};

template <typename Container>
constexpr typename std::enable_if_t<is_iterable<Container>{}(), std::ostream &> ostream_container_impl(std::ostream &os, const Container &v, typename Container::const_iterator cit) noexcept {
    return cit == std::cend(v) ? os : ostream_container_impl(os << *cit << (++cit == std::cend(v) ? "" : ","), v, cit);
}

template <typename Container>
constexpr typename std::enable_if_t<is_iterable<Container>{}(), std::ostream &> operator<<(std::ostream &os, const Container &v) noexcept {
    return ostream_container_impl(os << '{', v, std::cbegin(v)) << '}';
}
