#include <algorithm>
#include <iomanip>

#include "ostream_util.h"
#include "random_util.h"

int main() {
    DiscreteDistribution dis{1, 2, 3, 4};
    DiscreteDistribution dis1 = dis;
    std::cout << std::boolalpha << (dis == dis1) << std::endl;
    std::cout << (dis1 != dis) << std::endl;
    std::cout << dis << std::endl;
    std::mt19937 gen(time(nullptr));

    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(4) << dis << std::endl;

    std::vector<int> bucket(dis.param().size(), 0);
    for (int i = 0; i < 10000; i++) {
        bucket[dis(gen)]++;
    }
    std::cout << bucket << std::endl;
    operator==(dis, dis1);
    return 0;
}