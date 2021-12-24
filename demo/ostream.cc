#include <array>
#include <deque>
#include <forward_list>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ostream_ops.h"

int main() {
    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(2);
#if __cplusplus >= 201700L
    int a[2] = {1, 2};
    std::cout << a << std::endl;
    std::cout << array_wrapper(a) << std::endl;
    std::cout << reverse_container(array_wrapper(a)) << std::endl;
    std::cout << reverse_container(a) << std::endl;
    std::cout << reverse_container(std::vector<int>{1, 2, 3, 5, 8}) << std::endl;
#endif
    std::cout << std::pair<int, int>{} << std::endl;
    std::cout << std::tuple<>() << std::endl;
    std::cout << std::tuple<int, double, short, std::string>{2020, 12.0, 4, "22:24"} << std::endl;

    std::cout << std::array<int, 0>{} << std::endl;
    std::cout << std::array<int, 3>{1, 2, 3} << std::endl;

    std::cout << std::vector<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::vector<int>{} << std::endl;

    std::cout << std::deque<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::deque<int>{} << std::endl;

    std::cout << std::forward_list<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::forward_list<int>{} << std::endl;

    std::cout << std::list<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::list<int>{} << std::endl;

    std::cout << std::set<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::set<int>{} << std::endl;

    std::cout << std::unordered_set<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::unordered_set<int>{} << std::endl;

    std::cout << std::map<int, int>{{1, 2}, {3, 4}, {5, 6}} << std::endl;
    std::cout << std::map<int, int>{} << std::endl;

    std::cout << std::unordered_map<int, int>{{1, 2}, {3, 4}, {5, 6}} << std::endl;
    std::cout << std::unordered_map<int, int>{} << std::endl;

    std::cout << std::multiset<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::multiset<int>{} << std::endl;

    std::cout << std::multimap<int, int>{{1, 2}, {3, 4}, {5, 6}} << std::endl;
    std::cout << std::multimap<int, int>{} << std::endl;

    std::cout << std::unordered_multiset<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::unordered_multiset<int>{} << std::endl;

    std::cout << std::unordered_multimap<int, int>{{1, 2}, {3, 4}, {5, 6}} << std::endl;
    std::cout << std::unordered_multimap<int, int>{} << std::endl;

    std::cout << std::map<int, std::pair<int, double>>{{1, {1, 2.0}}} << std::endl;
    return 0;
}