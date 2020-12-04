#include <map>
#include <set>
#include <vector>

#include "ostream_util.h"

int main() {
    std::cout << std::boolalpha << !ContainerUtil<std::string>::have_ostream_operator() << std::endl;
    std::cout << std::tuple<int, double, short, std::string>{2020, 12.0, 4, "22:24"} << std::endl;
    std::cout << std::vector<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::map<int, std::pair<int, double>>{{1, {1, 2.0}}} << std::endl;
    std::cout << std::set<int>{1, 2, 3, 5, 8} << std::endl;
    return 0;
}