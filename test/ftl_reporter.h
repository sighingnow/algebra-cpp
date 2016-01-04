/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 *
 * Custom reporter for autocheck.
 */

#include <autocheck/autocheck.hpp>

class ftl_reporter : public autocheck::reporter {
public:
    virtual void success(size_t tests, size_t max_tests,
        size_t trivial, autocheck::distribution && dist) const {
        this->_correct = true;
        autocheck::report_success(std::clog, tests, max_tests, trivial, std::move(dist));
    }

    virtual void failure(size_t tests, const char* reason) const {
        std::ostringstream out;
        this->_correct = false; // const_cast<ftl_reporter *>(this)->_correct = true;
        autocheck::report_failure(out, tests, reason);
    }

// For checking the correctness of autocheck. True value means passing the test.
private:
    mutable bool _correct;
public:
    bool correct() { return this->_correct; }
};

