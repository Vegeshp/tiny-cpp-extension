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
template <typename _Tp>
struct IterUtil;

template <typename _T1, typename _T2>
constexpr std::ostream &
operator<<(std::ostream &os, const std::pair<_T1, _T2> &p) noexcept;

template <size_t _size = 0, typename... _Args>
constexpr typename std::enable_if_t<(_size >= sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept;

template <size_t _size = 0, typename... _Args>
constexpr typename std::enable_if_t<(_size < sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept;

template <typename _Container>
constexpr typename std::enable_if_t<!IterUtil<_Container>::have_ostream_operator(), std::ostream &>
operator<<(std::ostream &os, const _Container &v) noexcept;

template <typename _Container>
constexpr typename std::enable_if_t<!IterUtil<_Container>::have_ostream_operator(), std::ostream &>
ostream_container_impl(std::ostream &os, const _Container &v, typename _Container::const_iterator cit) noexcept;

// implementation

template <typename _T1, typename _T2>
constexpr std::ostream &operator<<(std::ostream &os, const std::pair<_T1, _T2> &p) noexcept {
    return os << '<' << p.first << ',' << p.second << '>';
}

template <size_t _size, typename... _Args>
constexpr typename std::enable_if_t<(_size >= sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept {
    return os;
}

template <size_t _size, typename... _Args>
constexpr typename std::enable_if_t<(_size < sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept {
    return operator<<<_size + 1, _Args...>(os << (_size ? "" : "<") << std::get<_size>(t) << (_size + 1 == sizeof...(_Args) ? '>' : ','), t);
}

template <typename _Tp>
struct IterUtil {  // derive from method-type-operator (feat. DevOps == Development & Operators)
   private:
    // type
    template <typename _Type>
    constexpr static std::true_type test_const_iterable(typename _Type::const_iterator *,
                                                        decltype(&_Type::cbegin),
                                                        decltype(&_Type::cend));
    template <typename _Type>
    constexpr static std::false_type test_const_iterable(...);

    template <typename _Type>
    constexpr static std::true_type test_const_reverse_iterable(typename _Type::const_reverse_iterator *,
                                                                decltype(&_Type::crbegin),
                                                                decltype(&_Type::crend));
    template <typename _Type>
    constexpr static std::false_type test_const_reverse_iterable(...);

    template <typename _Type>
    constexpr static decltype(std::declval<std::ostream &>() << std::declval<_Type>()) test_ostream_operator(_Type *x);
    template <typename _Type>
    constexpr static std::false_type test_ostream_operator(...);

   public:
    constexpr static bool is_const_iterable() noexcept {
        return std::is_same<std::true_type, decltype(test_const_iterable<_Tp>(nullptr, nullptr, nullptr))>{}();
    }
    constexpr static bool is_const_reverse_iterable() noexcept {
        return std::is_same<std::true_type, decltype(test_const_reverse_iterable<_Tp>(nullptr, nullptr, nullptr))>{}();
    }

    constexpr static bool have_ostream_operator() noexcept {
        return !std::is_same<std::false_type, decltype(test_ostream_operator<_Tp>(nullptr))>{}();
    }
};

template <typename _Container>
constexpr typename std::enable_if_t<!IterUtil<_Container>::have_ostream_operator(), std::ostream &>
ostream_container_impl(std::ostream &os, const _Container &v, typename _Container::const_iterator cit) noexcept {
    return cit == std::cend(v) ? os : ostream_container_impl(os << *cit << (++cit == std::cend(v) ? "" : ","), v, cit);
}

template <typename _Container>
constexpr typename std::enable_if_t<!IterUtil<_Container>::have_ostream_operator(), std::ostream &>
operator<<(std::ostream &os, const _Container &v) noexcept {
    return ostream_container_impl(os << '{', v, std::cbegin(v)) << '}';
}
