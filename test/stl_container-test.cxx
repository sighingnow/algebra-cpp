/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

/**
 * Unit test for list.
 */

#include <bandit/bandit.h>
#include <algebra/data/stl_container.hpp>
#include <autocheck/autocheck.hpp>
#include <iostream>
#include "./reporter.hpp"

go_bandit([]() {
    custom_reporter reporter;
    bandit::describe("STL container test: ", [&]() {
        bandit::before_each([&]() {

        });

        bandit::it("monoid::mempty() returns list with length 0", [&]() {
            AssertThat(algebra::monoid<std::list<int>>::mempty(), HasLength(0));
        });

        bandit::it("monoid::mappend(&, &)", [&]() {
            using algebra::operator^;
            auto s1 = std::list<int>{1, 2}, s2 = std::list<int>{3, 4};
            AssertThat(s1 ^ s2, Equals(std::list<int>{1, 2, 3, 4}));
        });

        bandit::it("monoid::mappend(&, &&)", [&]() {
            using algebra::operator^;
            auto s1 = std::list<int>{1, 2}, s2 = std::list<int>{3, 4};
            AssertThat(s1 ^ std::move(s2), Equals(std::list<int>{1, 2, 3, 4}));
        });

        bandit::it("monoid::mappend(&&, &)", [&]() {
            using algebra::operator^;
            auto s1 = std::list<int>{1, 2}, s2 = std::list<int>{3, 4};
            AssertThat(std::move(s1) ^ s2, Equals(std::list<int>{1, 2, 3, 4}));
        });

        bandit::it("monoid::mappend(&&, &&)", [&]() {
            using algebra::operator^;
            auto s1 = std::list<int>{1, 2}, s2 = std::list<int>{3, 4};
            AssertThat(std::move(s1) ^ std::move(s2),
                       Equals(std::list<int>{1, 2, 3, 4}));
        });

        // TODO how to rebind allocator ?

        // bandit::it("functor::fmap(a->b, &)", [&]() {
        //     using algebra::operator%;
        //     auto f = [](int x) { return float(x) + 0.5f; };
        //     auto l = std::list<int>{1, 2, 3, 4};
        //     auto r = f % l;
        //     AssertThat(r, Equals(std::list<float>{1.5f, 2.5f, 3.5f, 4.5f}));
        // });

        // bandit::it("functor::fmap(a->b, &&)", [&]() {
        //     using algebra::operator%;
        //     auto f = [](int x) { return float(x) + 0.5f; };
        //     auto r = f % std::list<int>{1, 2, 3, 4};
        //     AssertThat(r, Equals(std::list<float>{1.5f, 2.5f, 3.5f, 4.5f}));
        // });

        bandit::it("functor::fmap(a->a, &&)", [&]() {
            using algebra::operator%;
            auto f = [](int x) { return x + 1; };
            auto r = f % std::list<int>{1, 2, 3, 4};
            AssertThat(r, Equals(std::list<int>{2, 3, 4, 5}));
        });
    });
});

int main(int argc, char *argv[]) { return bandit::run(argc, argv); }
