/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

/**
 * Unit test for string.
 */

#include <bandit/bandit.h>
#include <algebra/data/string.hpp>
#include <autocheck/autocheck.hpp>
#include <iostream>
#include "./reporter.hpp"

go_bandit([]() {
    custom_reporter reporter;
    bandit::describe("String test: ", [&]() {
        bandit::before_each([&]() {

        });

        bandit::it("monoid::mempty() returns string with length 0", [&]() {
            AssertThat(algebra::monoid<std::string>::mempty(), HasLength(0));
        });

        bandit::it("monoid::mappend(&, &)", [&]() {
            using algebra::operator^;
            auto s1 = std::string("ab"), s2 = std::string("cd");
            AssertThat(s1 ^ s2, Equals(std::string("abcd")));
        });

        bandit::it("monoid::mappend(&, &&)", [&]() {
            using algebra::operator^;
            auto s1 = std::string("ab"), s2 = std::string("cd");
            AssertThat(s1 ^ std::move(s2), Equals(std::string("abcd")));
        });

        bandit::it("monoid::mappend(&&, &)", [&]() {
            using algebra::operator^;
            auto s1 = std::string("ab"), s2 = std::string("cd");
            AssertThat(std::move(s1) ^ s2, Equals(std::string("abcd")));
        });

        bandit::it("monoid::mappend(&&, &&)", [&]() {
            using algebra::operator^;
            auto s1 = std::string("ab"), s2 = std::string("cd");
            AssertThat(std::move(s1) ^ std::move(s2),
                       Equals(std::string("abcd")));
        });
    });
});

int main(int argc, char *argv[]) { return bandit::run(argc, argv); }
