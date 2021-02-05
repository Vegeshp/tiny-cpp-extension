#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "ostream_util.h"

// thanks for slongle's demand

struct Foo {
    template <typename... Args>
    Foo(Args &&...args) {
        std::cout << "Foo::size = " << sizeof...(Args) << std::endl;
        std::cout << std::make_tuple(args...) << std::endl;
    }
};

template <typename T, size_t... indices>
Foo impl(const std::vector<T> &a, std::index_sequence<indices...>) {
    return Foo(a[indices]...);
}

template <typename T, size_t N, typename Indices = std::make_index_sequence<N>>
Foo from_vec(const std::vector<T> &a) {
    return impl(a, Indices{});
}

int main() {
    Foo(from_vec<int, 5>(std::vector<int>{1, 1, 2, 3, 5}));
    return 0;
}
