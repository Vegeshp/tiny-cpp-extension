#pragma once

#include <iostream>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

// output pair<T, S>, tuple<A, B, ...>, container that supports iterator begin, end.
namespace __detail {

template <typename _Tp>
struct ContainerUtil {
   private:
    template <typename _Type>
    constexpr static std::true_type test_const_iterable(typename _Type::const_iterator *,
                                                        decltype(&_Type::cbegin),
                                                        decltype(&_Type::cend));

    template <typename _Type>
    constexpr static std::false_type test_const_iterable(...);

    template <typename _Type>
    constexpr static decltype(std::declval<std::ostream &>() << std::declval<_Type>()) test_ostream_operator(_Type *x);

    template <typename _Type>
    constexpr static std::false_type test_ostream_operator(...);

   public:
    constexpr static bool is_const_iterable() noexcept {
        return std::is_same<std::true_type, decltype(test_const_iterable<_Tp>(nullptr, nullptr, nullptr))>{}();
    }

    constexpr static bool have_ostream_operator() noexcept {
        return !std::is_same<std::false_type, decltype(test_ostream_operator<_Tp>(nullptr))>{}();
    }
};

template <typename _Container>
constexpr typename std::enable_if_t<!ContainerUtil<_Container>::have_ostream_operator(), std::ostream &>
ostream_container_impl(std::ostream &os, const _Container &v, typename _Container::const_iterator cit) noexcept;

};  // namespace __detail

// declaration

template <typename _T1, typename _T2>
constexpr std::ostream &operator<<(std::ostream &os, const std::pair<_T1, _T2> &p) noexcept;

template <size_t _size = 0, typename... _Args>
constexpr typename std::enable_if_t<(_size >= sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept;

template <size_t _size = 0, typename... _Args>
constexpr typename std::enable_if_t<(_size < sizeof...(_Args)), std::ostream &>
operator<<(std::ostream &os, const std::tuple<_Args...> &t) noexcept;

template <typename _Container>
constexpr typename std::enable_if_t<!__detail::ContainerUtil<_Container>::have_ostream_operator(), std::ostream &>
operator<<(std::ostream &os, const _Container &v) noexcept;

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
    // You cannot call `os << t` directly, only in this form. operator call
    //  `operator<< <template_list>(os_expression, content_expression)` calculation is in this order:
    //  1. os_expression 2. content_expression 3. operator
    return operator<<<_size + 1, _Args...>(os
                                               // if it's not the first, output blank as left border
                                               << (_size != 0 ? "" : "<")
                                               // concrete content
                                               << std::get<_size>(t)
                                               // if it's the last, output > as right border, else output broker
                                               << (_size + 1 == sizeof...(_Args) ? '>' : ','),
                                           t);
}

// We cannot just implement inside, since binary operator cannot hide more parameter in one function, so a helper
// function is necessary
template <typename _Container>
constexpr typename std::enable_if_t<!__detail::ContainerUtil<_Container>::have_ostream_operator(), std::ostream &>
operator<<(std::ostream &os, const _Container &v) noexcept {
    return __detail::ostream_container_impl(os << '{', v, std::cbegin(v)) << '}';
}

template <typename _Container>
constexpr typename std::enable_if_t<!__detail::ContainerUtil<_Container>::have_ostream_operator(), std::ostream &>
__detail::ostream_container_impl(std::ostream &os,
                                    const _Container &v,
                                    typename _Container::const_iterator cit) noexcept {
    // this is a [cbegin, cend) recursive one-function implementation.
    return cit == std::cend(v)
               ? os
               : ostream_container_impl(os << *cit << (++cit == std::cend(v) ? "" : ","), v, cit);
}