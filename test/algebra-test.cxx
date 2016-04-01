#include <iostream>

#include <bandit/bandit.h>
#include <autocheck/autocheck.hpp>

#include <algebra/data/monoid.hpp>
#include "./reporter.hpp"

template <typename A, typename B = A>
bool add(A const &a, A const &b) {
    return a + b == b + a;
}

go_bandit([]() {
    custom_reporter reporter;
    bandit::describe("Hello world:", [&]() {
        bandit::before_each([&]() {

        });

        bandit::it("add correct", [&]() {
            autocheck::check<int, int>(add<int, int>, 100,
                                       autocheck::make_arbitrary<int, int>(),
                                       reporter);
            AssertThat(reporter.correct(), IsTrue());
        });
    });
});

int main(int argc, char *argv[]) { return bandit::run(argc, argv); }
