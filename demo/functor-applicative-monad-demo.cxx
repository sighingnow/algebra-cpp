/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#include <algebra/control/applicative.hpp>
#include <algebra/control/functor.hpp>
#include <algebra/control/monad.hpp>
#include <algebra/data/stl_container.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

void functor_demo() {
    std::cout << "Functor examples------------" << std::endl;
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
    std::cout << std::endl;
}

void applicative_demo() {
    std::cout << "Applicative examples------------" << std::endl;

    auto pure_container = algebra::applicative<std::list<int>>::pure({2});
    std::cout << "length of pure container: " << pure_container.size()
              << std::endl;

    std::list<std::function<int(int)>> fs{[](int x) { return x - 1; },
                                          [](int x) { return x + 1; }};

    using algebra::operator*;
    // auto r = algebra::applicative<std::list<int>>::ap(
    //         fs, std::list<int>{1, 2, 3, 4});
    auto r = fs * std::list<int>{1, 2, 3, 4};
    std::for_each(std::begin(r), std::end(r),
                  [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;
}

void monad_demo() {
    std::cout << "Monad examples------------" << std::endl;

    std::list<int> l = {1, 2, 3, 4};
    std::cout << "origin list l: " << std::endl;
    std::for_each(std::begin(l), std::end(l),
                  [](int const& n) { std::cout << n << " "; });
    std::cout << std::endl;

    using algebra::operator>>=;
    auto f = [](int const& x) {
        return std::list<float>{x + 0.5f, x + 1.5f, x + 2.5f, x + 3.5f};
    };
    auto k = std::list<int>{1, 2, 3} >>= f;
    std::for_each(std::begin(k), std::end(k),
                  [](int const& n) { std::cout << n << " "; });
}

void functor_applicative_monad_demo() {
    functor_demo();
    applicative_demo();
    monad_demo();
}
