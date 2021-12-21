#include <algorithm>
#include <cassert>
#include <iomanip>

#include "alias_method.h"
#include "ostream_ops.h"

int main() {
    AliasMethod dis{1, 2, 3, 4};
    std::cout << dis << std::endl;

    AliasMethod dis1 = dis;
    assert(dis == dis1);
    assert(!(dis1 != dis));

    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(4) << dis << std::endl;

    std::mt19937 gen(time(nullptr));
    std::vector<int> bucket(dis.param().size(), 0);
    for (int i = 0; i < 10000; i++) {
        bucket[dis(gen)]++;
    }
    std::cout << bucket << std::endl;
    return 0;
}