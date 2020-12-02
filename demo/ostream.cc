#include <map>
#include <set>
#include <vector>

#include "ostream_util.h"

int main() {
    std::cout << std::boolalpha << is_iterable<std::string>{}() << std::endl;
    std::tuple<int, int, int, std::string> tuple{2020, 11, 9, "14:18"};
    std::cout << tuple << std::endl;
    std::vector<int> v{1, 2, 3, 5, 8};
    std::cout << v << std::endl;
    std::map<int, std::pair<int, double>> map{{1, {1, 2.0}}};
    std::cout << map << std::endl;
    std::set<int> set{1, 2, 3, 5, 8};
    std::cout << set << std::endl;
    return 0;
}