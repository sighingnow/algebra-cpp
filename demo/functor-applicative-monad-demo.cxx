/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#include <algebra/control/applicative.hpp>
#include <algebra/control/functor.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

void functor_demo() {
    std::cout << "Functor examples------------" << std::endl;
    std::cout << std::endl;
}

void applicative_demo() {
    std::cout << "Applicative examples------------" << std::endl;
    std::cout << std::endl;
}

void functor_applicative_monad_demo() {
    functor_demo();
    applicative_demo();
}
