/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#include <algebra/control/functor.hpp>
#include <algebra/data/stl_container.hpp>
#include <functional>
#include <iostream>
#include <vector>

void stl_container_demo() {
    std::cout << "List examples------------" << std::endl;
    std::cout << "Length of empty list<int>: "
              << algebra::monoid<std::list<int>>::mempty().size() << std::endl;

    std::list<int> l = {1, 2, 3, 4};
    std::cout << "origin list l: " << std::endl;
    std::for_each(std::begin(l), std::end(l),
                  [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;

    // using algebra::operator%;
    // auto f = [](int const& x) {
    //     return x * x + x + 1;
    // };
    // auto r = f % l;  // list as a functor.
    // std::for_each(std::begin(r), std::end(r),
    //               [](int const& n) { std::cout << n << " "; });

    // auto k = algebra::applicative<std::list<int>>::pure({2});

    using algebra::operator>>=;
    auto f = [](int const& x) {
        return std::list<int>{x, x + 1, x + 2, x + 3};
    };
    auto k = std::list<int>{1, 2, 3} >>= f;
    std::for_each(std::begin(k), std::end(k),
                  [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;
}
