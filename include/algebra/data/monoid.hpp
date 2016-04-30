/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_DATA_MONOID_HPP__
#define __ALGEBRA_DATA_MONOID_HPP__

#include "../basic/type_concepts.hpp"
#include "../basic/type_operation.hpp"

/**
 * Monoid, the following laws are hold:
 *  + right identity law
 *  + left identity law
 *  + associativity law.
 */
namespace algebra {

    template <typename M>
    struct monoid {
        static constexpr bool instance = false;
        /**
         * Minimal complete definitions.
         */
        static M mempty();
        static M mappend(M ma, M mb);
    };

    /**
     * Monoid type predication.
     */
    template <typename M>
    struct Monoid {
        static constexpr bool value = monoid<M>::instance;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Operator overloading for `mappend` method.
     */
    template <typename MA, typename MB, typename M = PlainType<MA>,
              typename = Requires<Monoid<M>{} && std::is_same<M, PlainType<MB>>::value>>
    M operator^(MA &&ma, MB &&mb) {
        return monoid<M>::mappend(std::forward<MA>(ma), std::forward<MB>(mb));
    }

    /**
     * Monoid for numbers, use ordinary `+` as `mappend`.
     */
    template <typename N>
    struct sum_monoid {
        static constexpr bool instance = true;

        N value;

        // Constructors.
        constexpr sum_monoid() noexcept : value(0) {}
        constexpr sum_monoid(N n) noexcept(std::is_nothrow_copy_constructible<N>::value)
                : value(n) {}

        // Implicit cast to unboxed value of type `N`.
        constexpr operator N() const noexcept { return value; }

        // '+' definition.
        constexpr sum_monoid operator+(const sum_monoid &x) const
                noexcept(std::is_nothrow_move_constructible<N>::value) {
            return std::move(sum_monoid(value + x.value));
        }
    };

    template <typename N>
    constexpr sum_monoid<N> sum(N n) noexcept(
            std::is_nothrow_constructible<sum_monoid<N>, N>::value) {
        return sum_monoid<N>(n);
    }

    /**
     * Instance sum_monoid as a monoid type.
     */
    template <typename N>
    struct monoid<sum_monoid<N>> {
        static constexpr bool instance = true;

        static constexpr sum_monoid<N> mempty() noexcept(
                std::is_nothrow_constructible<sum_monoid<N>, N>::value) {
            return sum(N(0));
        }
        static constexpr sum_monoid<N> mappend(const sum_monoid<N> &a, const sum_monoid<N> &b) {
            return a + b;
        }
    };

    /**
     * Monoid for numbers, use ordinary `*` as `mappend`.
     */
    template <typename N>
    struct prod_monoid {
        static constexpr bool instance = true;

        N value;

        // Constructors.
        constexpr prod_monoid() noexcept : value(0) {}
        constexpr prod_monoid(N n) noexcept(std::is_nothrow_copy_constructible<N>::value)
                : value(n) {}

        // Implicit cast to unboxed value of type `N`.
        constexpr operator N() const noexcept { return value; }

        // '*' definition.
        constexpr prod_monoid operator*(const prod_monoid &x) const
                noexcept(std::is_nothrow_move_constructible<N>::value) {
            return std::move(prod_monoid(value * x.value));
        }
    };

    template <typename N>
    constexpr prod_monoid<N> prod(N n) noexcept(
            std::is_nothrow_constructible<prod_monoid<N>, N>::value) {
        return prod_monoid<N>(n);
    }

    /**
     * Instance prod_monoid as a monoid type.
     */
    template <typename N>
    struct monoid<prod_monoid<N>> {
        static constexpr bool instance = true;

        static constexpr prod_monoid<N> mempty() noexcept(
                std::is_nothrow_constructible<prod_monoid<N>, N>::value) {
            return prod(N(1));
        }
        static constexpr prod_monoid<N> mappend(const prod_monoid<N> &a, const prod_monoid<N> &b) {
            return a * b;
        }
    };
};

#endif /* __ALGEBRA_DATA_MONOID_HPP__ */
