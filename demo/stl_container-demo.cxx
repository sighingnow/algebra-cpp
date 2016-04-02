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

    using algebra::operator%;
    auto f = [](int const& x) -> float { return x * x + x + 1.5f; };
    auto r = f % l;  // list as a functor.
    std::for_each(std::begin(r), std::end(r),
                  [](float const& n) { std::cout << n << " "; });

    std::cout << std::boolalpha << (r == std::list<float>{3.5, 7.5, 13.5, 21.5})
              << std::noboolalpha << std::endl;

    // auto k = algebra::applicative<std::list<int>>::pure({2});

    // using algebra::operator>>=;
    // auto f = [](int const& x) {
    //     return std::list<float>{x + 0.5f, x + 1.5f, x + 2.5f, x + 3.5f};
    // };
    // auto k = std::list<int>{1, 2, 3} >>= f;
    // std::for_each(std::begin(k), std::end(k),
    //               [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;
}
