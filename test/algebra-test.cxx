#include <iostream>

#include <autocheck/autocheck.hpp>
#include <bandit/bandit.h>


template <typename A, typename B = A>
bool add(A const & a, A const & b) {
    return a + b == b + a;
}

/**
 * Custom reporter for autocheck.
 */
class algebra_reporter : public autocheck::reporter {
public:
    virtual void success(size_t tests, size_t max_tests,
        size_t trivial, autocheck::distribution && dist) const {
        this->_correct = true;
        autocheck::report_success(std::clog, tests, max_tests, trivial, std::move(dist));
    }
    virtual void failure(size_t tests, const char* reason) const {
        std::ostream & out = std::cout;
        this->_correct = false; // const_cast<algebra_reporter *>(this)->_correct = true;
        out << std::endl; // to separate result of two test cases.
        autocheck::report_failure(out, tests, reason);
    }
// For checking the correctness of autocheck. True value means passing the test.
private:
    mutable bool _correct;
public:
    bool correct() { return this->_correct; }
};

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
