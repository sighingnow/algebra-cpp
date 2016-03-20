/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#include <iostream>
#include <algebra/data/monoid.hpp>

void monoid_demo() {
    std::cout << "Monoid examples------------" << std::endl;

    algebra::sum_monoid<int>
            a = 3,
            b = 4, c = algebra::monoid<algebra::sum_monoid<int>>::mempty();
    algebra::prod_monoid<float> x = 3.f, y = 2.5f, z = algebra::prod(3.0f);
    std::cout << (a ^ b ^ c) << "  " << (x ^ y ^ z) << std::endl;
}
