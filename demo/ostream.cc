#include "ostream_util.h"

int main() {
    std::tuple<int, int, int, std::string> tuple{2020, 11, 9, "14:18"};
    std::cout << tuple << std::endl;
    return 0;
}