/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#ifndef __ALGEBRA_H_DATA_STRING_HPP__
#define __ALGEBRA_H_DATA_STRING_HPP__

#include <string>
#include "../basic/type_concepts.hpp"
#include "../basic/type_operation.hpp"
#include "../data/monoid.hpp"

/**
 * String, monoid.
 */
namespace algebra {
template <typename... Ts>
struct monoid<std::basic_string<Ts...>> {
    static std::basic_string<Ts...> mempty() {
        return std::basic_string<Ts...>{};
    }
    static std::basic_string<Ts...> mappend(
            const std::basic_string<Ts...>& s1,
            const std::basic_string<Ts...>& s2) {
        return s1 + s2;
    }
    static std::basic_string<Ts...> mappend(
            std::basic_string<Ts...>&& s1, const std::basic_string<Ts...>& s2) {
        // s1 += s2;
        s1.insert(std::end(s1), std::begin(s2), std::end(s2));
        return std::move(s1);
    }
    static std::basic_string<Ts...> mappend(const std::basic_string<Ts...>& s1,
                                            std::basic_string<Ts...>&& s2) {
        s2.insert(std::begin(s2), std::begin(s1), std::end(s1));
        return std::move(s2);
    }
    static std::basic_string<Ts...> mappend(std::basic_string<Ts...>&& s1,
                                            std::basic_string<Ts...>&& s2) {
        std::move(std::begin(s2), std::end(s2), std::back_inserter(s1));
        return std::move(s1);
    }
    static constexpr bool instance = true;
};
};

#endif /* __ALGEBRA_H_DATA_STRING_HPP__ */
