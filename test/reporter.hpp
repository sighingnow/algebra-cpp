/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#include <iostream>
#include <autocheck/autocheck.hpp>

/**
 * Custom reporter for autocheck.
 */
class custom_reporter : public autocheck::reporter {
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
    mutable bool _correct = true;
public:
    bool correct() { return this->_correct; }
};

