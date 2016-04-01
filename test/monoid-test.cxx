/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

/**
 * Unit test for monoid.
 */

#include <bandit/bandit.h>
#include <autocheck/autocheck.hpp>

#include <algebra/data/monoid.hpp>
#include "./reporter.hpp"

template <typename A, typename B = A>
bool add(A const &a, A const &b) {
    return a + b == b + a + a;
}

go_bandit([]() {
    custom_reporter reporter;
    bandit::describe("Monoid test: ", [&]() {

        auto id_sum = algebra::monoid<algebra::sum_monoid<int>>::mempty();
        auto id_prod = algebra::monoid<algebra::prod_monoid<int>>::mempty();

        bandit::before_each([&]() {});

        bandit::it("sum_monoid right identity law: ", [&]() {
            autocheck::check<int>(
                    [&](int k) -> bool {
                        return k == (algebra::sum(k) ^ id_sum);
                    },
                    100, autocheck::make_arbitrary<int>(), reporter);
            AssertThat(reporter.correct(), IsTrue());
        });

        bandit::it("sum_monoid left identity law: ", [&]() {
            autocheck::check<int>(
                    [&](int k) -> bool {
                        return k == (id_sum ^ algebra::sum(k));
                    },
                    100, autocheck::make_arbitrary<int>(), reporter);
            AssertThat(reporter.correct(), IsTrue());
        });

        bandit::it("sum_monoid law of associativity: ", [&]() {
            autocheck::check<int, int, int>(
                    [&](int a, int b, int c) -> bool {
                        algebra::sum_monoid<int> ka = a, kb = b, kc = c;
                        return (ka ^ (kb ^ kc)) == ((ka ^ kb) ^ kc);
                    },
                    100, autocheck::make_arbitrary<int, int, int>(), reporter);
        });

        bandit::it("prod_monoid right identity law: ", [&]() {
            autocheck::check<int>(
                    [&](int k) -> bool {
                        return k == (algebra::prod(k) ^ id_prod);
                    },
                    100, autocheck::make_arbitrary<int>(), reporter);
            AssertThat(reporter.correct(), IsTrue());
        });

        bandit::it("prod_monoid left identity law: ", [&]() {
            autocheck::check<int>(
                    [&](int k) -> bool {
                        return k == (id_prod ^ algebra::prod(k));
                    },
                    100, autocheck::make_arbitrary<int>(), reporter);
            AssertThat(reporter.correct(), IsTrue());
        });

        bandit::it("prod_monoid law of associativity: ", [&]() {
            autocheck::check<int, int, int>(
                    [&](int a, int b, int c) -> bool {
                        algebra::prod_monoid<int> ka = a, kb = b, kc = c;
                        return (ka ^ (kb ^ kc)) == ((ka ^ kb) ^ kc);
                    },
                    100, autocheck::make_arbitrary<int, int, int>(), reporter);
        });
    });
});

int main(int argc, char *argv[]) { return bandit::run(argc, argv); }
