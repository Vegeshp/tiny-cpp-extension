#include <iomanip>
#include <map>
#include <set>
#include <vector>

#include "ostream_ops.h"

int main() {
    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(2);
    std::cout << std::pair<int, int>{} << std::endl;
    std::cout << std::tuple<>() << std::endl;
    std::cout << std::tuple<int, double, short, std::string>{2020, 12.0, 4, "22:24"} << std::endl;
    std::cout << std::vector<int>{1, 2, 3, 5, 8} << std::endl;
    std::cout << std::vector<int>{} << std::endl;
    std::cout << std::map<int, std::pair<int, double>>{{1, {1, 2.0}}} << std::endl;
    std::cout << std::set<int>{1, 2, 3, 5, 8} << std::endl;
    return 0;
}