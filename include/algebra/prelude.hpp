/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_PRELUDE_HPP__
#define __ALGEBRA_PRELUDE_HPP__

#include <functional>
#include "./basic/type_concepts.hpp"
#include "./basic/type_operation.hpp"

/**
 * Prelude level functions.
 */
namespace algebra {
    /**
     * Identity function.
     * In haskell:
     *      id x = x.
     */
    constexpr struct id_impl {
        template <typename T>
        constexpr auto operator()(T &&t) const noexcept -> decltype(std::forward<T>(t)) {
            return std::forward<T>(t);
        }
    } _id{};

    /**
     * Constant function.
     * In haskell:
     *      const x _ = x
     */
    constexpr struct const_impl {
        template <typename T, typename U>
        // TODO: make it curried.
        constexpr auto operator()(T &&t, U &&) const noexcept -> decltype(std::forward<T>(t)) {
            return std::forward<T>(t);
        }
    } _const{};
};

#endif /* __ALGEBRA_PRELUDE_HPP__ */
