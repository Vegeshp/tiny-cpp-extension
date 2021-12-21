#include <iostream>
#include <vector>

#include "type_name.h"

int main() {
    std::cout << from_type(int) << std::endl;
    std::cout << from_type(int&) << std::endl;
    std::cout << from_type(int&&) << std::endl;
    std::cout << from_type(int[2]) << std::endl;

    std::cout << from_var(std::vector<int>{}) << std::endl;
    return 0;
}