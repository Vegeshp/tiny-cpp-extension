#include <iostream>
#include <vector>

#include "type_name.h"

int f() { return 0; }

int main() {
    {
        int x = 1;
        std::cout << var_type(x) << ", " << category_type(x) << std::endl;
        std::cout << var_type(0) << ", " << category_type(0) << std::endl;
        std::cout << var_type(f()) << ", " << category_type(f()) << std::endl;
    }
    std::cout << std::endl;
    {
        int x = 1;
        int& rx = x;
        int&& rrx = std::move(x);
        const int cx = x;
        const int& crx = x;
        std::cout << var_type(rx) << ", " << category_type(rx) << std::endl;
        std::cout << var_type(rrx) << ", " << category_type(rrx) << std::endl;
        std::cout << var_type(cx) << ", " << category_type(cx) << std::endl;
        std::cout << var_type(crx) << ", " << category_type(crx) << std::endl;
    }
    std::cout << std::endl;
    {
        int x = 0;
        int* px = &x;
        const int* pcx = px;
        int* const cpx = px;
        std::cout << var_type(px) << ", " << category_type(px) << std::endl;
        std::cout << var_type(pcx) << ", " << category_type(pcx) << std::endl;
        std::cout << var_type(cpx) << ", " << category_type(cpx) << std::endl;
        std::cout << var_type((int*)(nullptr)) << ", " << category_type((int*)(nullptr)) << std::endl;
    }
    std::cout << std::endl;
    {
        int a[2];
        int(&ra)[2] = a;
        int(*pa)[2] = &a;
        const int(&cra)[2] = a;
        const int(*cpa)[2] = &a;
        const int(*const cpcpa)[2] = pa;
        std::cout << var_type(a) << ", " << category_type(a) << std::endl;
        std::cout << var_type(ra) << ", " << category_type(ra) << std::endl;
        std::cout << var_type(pa) << ", " << category_type(pa) << std::endl;
        std::cout << var_type(cra) << ", " << category_type(cra) << std::endl;
        std::cout << var_type(cpa) << ", " << category_type(cpa) << std::endl;
        std::cout << var_type(cpcpa) << ", " << category_type(cpcpa) << std::endl;
    }
    return 0;
}
