/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#include <algebra/data/string.hpp>
#include <functional>
#include <iostream>

void string_demo() {
    std::cout << "String examples------------" << std::endl;
    std::cout << "Length of empty string: "
              << algebra::monoid<std::string>::mempty().size() << std::endl;
    std::cout << std::endl;
}
