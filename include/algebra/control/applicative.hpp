/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_H_CONTROL_APPLICATIVE_HPP__
#define __ALGEBRA_H_CONTROL_APPLICATIVE_HPP__

#include "../basic/type_concepts.hpp"
#include "../basic/type_operation.hpp"
#include "../control/functor.hpp"

/**
 * Applicative functor.
 *
 * Applicative laws:
 *  + identity: pure id <*> v = v
 *  + composition: pure (.) <*> u <*> v <*> w = u <*> (v <*> w)
 *  + homomorphism: pure f <*> pure x = pure (f x)
 *  + interchange: u <*> pure y = pure ($ y) <*> u
 *
 * All applicative functors are also functors.
 */
namespace algebra {

// Forward declaration of monad.
// Then the `ap` of applicative functor can be implemented using `monad<M>::ap`.
template <typename M>
struct monad;

template <typename F, typename = Requires<Functor<F>::value>>
struct applicative {
    using T = ValueType<F>;

    template <typename U>
    using _F = Rebind<F, U>;

    /**
     * Minimal complete definition.
     */

    // Encapsulate a value into an applicative context.
    // In Haksell:
    //      pure :: applicative f => a -> f a
    static constexpr _F<T> pure(const T &x) { return monad<F>::pure(x); }

    static constexpr _F<T> pure(T &&x) { return monad<F>::pure(std::move(x)); }

    // Apply Contextaulised function to applicative functor.
    template <typename Ff, typename Fn = ValueType<PlainType<Ff>>,
              typename U = ResultOf<Fn(T)>>
    static _F<U> ap(Ff &&fn, const _F<T> &f) {
        return monad<F>::ap(std::forward<Ff>(fn), f);
    }

    template <typename Ff, typename Fn = ValueType<PlainType<Ff>>,
              typename U = ResultOf<Fn(T)>>
    static _F<U> ap(Ff &&fn, _F<T> &&f) {
        return monad<F>::ap(std::forward<Ff>(fn), std::move(f));
    }

    /**
     * Other useful methods.
     */

    // Lift a function to inner value of an applicative functor.
    // In Haskell:
    //      liftA :: applicative f => (a -> b) -> f a -> f b
    template <typename Fn, typename U = ResultOf<Fn(T)>>
    static _F<U> liftA(Fn &&fn, const _F<T> &f) {
        return functor<F>::fmap(std::forward<Fn>(fn), f);
    }

    template <typename Fn, typename U = ResultOf<Fn(T)>>
    static _F<U> liftA(Fn &&fn, _F<T> &&f) {
        return functor<F>::fmap(std::forward<Fn>(fn), std::move(f));
    }

    // Just a generic class.
    static constexpr bool instance = monad<F>::instance;
};

/**
 * Applicative functor type predication.
 */
template <typename F>
struct Applicative {
    static constexpr bool value = applicative<F>::instance;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Default definition for `pure`, `ap`, and `liftA`.
 */

// In Haskell:
//      pure :: Applicative f => a -> f a.
//
// The type `F` must be the type with either a suitable unary constructor, or a
// constructor taking an initialiser list.
template <typename F
          // TODO why this constrain can't hold on ?
          //, typename = Requires<std::is_constructible<F, ValueType<F>>::value>
          >
struct default_pure {
    using T = ValueType<F>;

    static constexpr F pure(const T &t) noexcept(
            std::is_nothrow_constructible<F, const T &>::value) {
        return F{t};
    }

    static constexpr F pure(T &&t) noexcept(
            std::is_nothrow_constructible<F, T &&>::value) {
        return F{std::move(t)};
    }
};

/**
 * Operator overloading for applicative functor operation.
 */

// Overloading `*` as `ap`.
template <typename Ff, typename F, typename Fn = PlainType<Ff>,
          typename _F = PlainType<F>,
          typename = Requires<Applicative<_F>::value>,
          typename = Requires<SameTemplate<Fn, _F>::value>>
auto operator*(Ff &&u, F &&v) {
    return applicative<_F>::ap(std::forward<Ff>(u), std::forward<F>(v));
}

template <typename Ff, typename F, typename Fn = PlainType<Ff>,
          typename _F = PlainType<F>,
          typename = Requires<Applicative<_F>::value>,
          typename = Requires<SameTemplate<Fn, _F>::value>>
auto operator*(Ff &&u, const F &v) {
    return applicative<_F>::ap(std::forward<Ff>(u), v);
}
};

#endif /* __ALGEBRA_H_CONTROL_APPLICATIVE_HPP__ */
