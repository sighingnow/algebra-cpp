/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#include <algebra/data/functor.hpp>
#include <algebra/data/list.hpp>
#include <functional>
#include <iostream>
#include <vector>

void list_demo() {
    std::cout << "List examples------------" << std::endl;
    std::cout << "Length of empty list<int>: "
              << algebra::monoid<std::list<int>>::mempty().size() << std::endl;
    std::list<int> l = {1, 2, 3, 4};
    std::for_each(std::begin(l), std::end(l),
                  [](int const& n) { std::cout << n << " "; });
    auto f = [](int const& k) { return k + 1; };
    using algebra::operator%;
    auto r = f % l;  // list as a functor.
    std::for_each(std::begin(r), std::end(r),
                  [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;
}
