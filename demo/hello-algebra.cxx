/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#include <algebra.hpp>
#include <iostream>

void monoid_demo();
void string_demo();
void list_demo();
void functor_applicative_monad_demo();

int main() {
    std::cout << "Hello algebra !" << std::endl;

    monoid_demo();
    string_demo();
    list_demo();
    functor_applicative_monad_demo();

    return 0;
}
