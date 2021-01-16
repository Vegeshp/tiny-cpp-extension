#include "type_util.h"

template <typename T>
void test() {
    std::cout << ++count << '\t' << __detail::get_name<T>() << std::endl;
    if (!(count & 3)) {
        std::cout << std::endl;
    }
}

int cmp0(int, int) { return 0; }

int main() {
    std::cout.setf(std::ios::boolalpha);

    test<int>();                 // 1
    test<const int>();           // 2
    test<volatile int>();        // 3
    test<const volatile int>();  // 4

    test<int*>();                 // 5
    test<int* const>();           // 6
    test<int* volatile>();        // 7
    test<int* const volatile>();  // 8

    test<const int*>();                 // 9
    test<const int* const>();           // 10
    test<const int* volatile>();        // 11
    test<const int* const volatile>();  // 12

    test<volatile int*>();                 // 13
    test<volatile int* const>();           // 14
    test<volatile int* volatile>();        // 15
    test<volatile int* const volatile>();  // 16

    test<const volatile int*>();                 // 17
    test<const volatile int* const>();           // 18
    test<const volatile int* volatile>();        // 19
    test<const volatile int* const volatile>();  // 20

    test<int[3]>();       // 21
    test<int(*)[3]>();    // 22
    test<int(&)[3]>();    // 23
    test<int(&&)[3]>();   // 24
    test<int(*&)[3]>();   // 25
    test<int(*&&)[3]>();  // 26
    test<int&>();         // 27
    test<int&&>();        // 28

    auto f = [](int a, bool b, char c) -> bool { return true; };
    auto g = [](int a, double b) -> int { return 0; };
    int cmp(int, int);
    bool check(int, double, long long);

    test<decltype(f)>();      // 29
    test<decltype(g)>();      // 30
    test<decltype(cmp)>();    // 31
    test<decltype(check)>();  // 32

    test<decltype(&f)>();      // 33
    test<decltype(&g)>();      // 34
    test<decltype(&cmp)>();    // 35
    test<decltype(&check)>();  // 36

    int (*cmp1)(int, int);
    int (*cmp2)();
    int (&cmp3)(int, int) = cmp0;
    int (*&cmp4)() = cmp2;
    int(&&cmp5)(int, int) = std::move(cmp3);
    int (*&&cmp6)() = std::move(cmp4);
    void (*base1)() = []() {};
    void (*base2)(int, int) = [](int a, int b) {};

    test<decltype(cmp1)>();  // 37
    test<decltype(cmp2)>();  // 38
    test<decltype(cmp3)>();  // 39
    test<decltype(cmp4)>();  // 40

    test<decltype(cmp5)>();   // 41
    test<decltype(cmp6)>();   // 42
    test<decltype(base1)>();  // 43
    test<decltype(base2)>();  // 44

    return 0;
}
