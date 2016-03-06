#include <iostream>

#include <autocheck/autocheck.hpp>
#include <bandit/bandit.h>
#include "algebra_reporter.h"

template <typename A, typename B = A>
bool add(A const & a, A const & b) {
    return a + b == b + a;
}

go_bandit([](){
    algebra_reporter reporter;
    bandit::describe("Hello world:", [&](){
        bandit::before_each([&](){

        });

        bandit::it("add correct", [&](){
            autocheck::check<int, int>(add<int, int>, 100, autocheck::make_arbitrary<int, int>(), reporter);
            AssertThat(reporter.correct(), IsTrue());
        });
    });
});

int main(int argc, char* argv[])
{
    return bandit::run(argc, argv);
}
