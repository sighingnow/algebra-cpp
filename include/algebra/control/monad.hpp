/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_CONTROL_MONAD_HPP__
#define __ALGEBRA_CONTROL_MONAD_HPP__

#include "../basic/type_operation.hpp"

namespace algebra {

/**
 * Monad: represents sequenceable computations in a context.
 * Monad laws:
 *  + pure x >>= f = f x
 *  + m >>= pure = m
 *  + (m >>= f) >>= g = m >>= ((\x -> f x) >>= g)
 */
template <typename M>
struct monad {
    // Just a generic type class, not monad instance.
    static constexpr bool instance = false;

    // Provide type signatures for this specialised monad.
    template <typename U>
    using _M = Rebind<M, U>;

    // The inner value type in the type parameter `_M`.
    using T = ValueType<M>;

    /**
     * Minimal complete definition.
     */

    // pure: monad m => a -> m a.
    static _M<T> pure(const T &);

    // bind: monad m => m a -> (a -> m b) -> m b.
    template <typename F, typename U = ValueType<ResultOf<F(T)>>>
    static _M<U> bind(const _M<T> &m, F &&f);

    /**
     * Other useful methods.
     */

    // join: monad m => m (m a) => m a.
    static _M<T> join(const _M<_M<T>> &m);

    // ap (apply): monad m => m (a -> b) -> m a -> m b.
    template <typename F, typename U = ResultOf<F(T)>>
    static _M<U> ap(const _M<F> &fn, const _M<T> &m);

    // liftM: monad m => (a -> b) -> m a -> m b.
    template <typename F, typename U = ResultOf<F(T)>>
    static _M<U> liftM(F &&fn, const _M<T> &m);
};

/**
 * Monad type predication.
 */
template <typename M>
struct Monad {
    static constexpr bool value = monad<M>::instance;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Identity functor.
 */
constexpr struct identity {
    template <typename T>
    constexpr auto operator()(T &&t) const noexcept
            -> decltype(std::forward<T>(t)) {
        return std::forward<T>(t);
    }
} id{};

/**
 * Default definition for `pure`, `bind`, `map` and `join`.
 */

// pure: monad m => a -> m a.
template <typename M>
struct default_pure {
    using T = ValueType<M>;
    static constexpr M pure(const T &t) noexcept(
            std::is_nothrow_constructible<M, const T &>::value) {
        return M{t};
    }
    static constexpr M pure(T &&t) noexcept(
            std::is_nothrow_constructible<M, T &&>::value) {
        return M{std::move(t)};
    }
};

// bind: monad m => m a -> (a -> m b) -> m b.
template <typename M>
struct default_bind {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    template <typename F, typename U = ValueType<ResultOf<F(T)>>>
    static constexpr _M<U> bind(const _M<T> &m, F &&f) {
        return monad<_M<U>>::join(monad<_M<T>>::map(std::forward<F>(f), m));
    }
    template <typename F, typename U = ValueType<ResultOf<F(T)>>>
    static constexpr _M<U> bind(_M<T> &&m, F &&f) {
        return monad<_M<U>>::join(
                monad<_M<T>>::map(std::forward<F>(f), std::move(m)));
    }
};

// join: monad m => m (m a) => m a.
// default `join` in haskell:
//  join x =  x >>= id
template <typename M>
struct default_join {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    static constexpr _M<T> join(const _M<_M<T>> &m) {
        return monad<_M<_M<T>>>::bind(m, id);
    }

    static constexpr _M<T> join(_M<_M<T>> &&m) {
        return monad<_M<_M<T>>>::bind(std::move(m), id);
    }
};

// ap (apply): monad m => m (a -> b) -> m a -> m b.
// default `ap` in haskell:
//  ap m1 m2 = do { x1 <- m1; x2 <- m2; return (x1 x2) }
template <typename M>
struct default_ap {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    template <typename MF, typename _MF = PlainType<MF>,
              typename F = ValueType<_MF>, typename U = ResultOf<F(T)>>
    static constexpr _M<U> ap(MF &&f, M m) {
        return monad<_MF>::bind(std::forward<MF>(f), [m](F fn) {
            return monad<M>::bind(
                    m, [fn](const T &t) { return monad<_M<U>>::pure(fn(t)); });
        });
    }
};

// liftM: monad m => (a -> b) -> m a -> m b.
// default `liftM` in haskell:
//  liftM f m1 = do { x1 <- m1; return (f x1) }
template <typename M>
struct default_liftM {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    template <typename F, typename U = ResultOf<F(T)>>
    static constexpr _M<U> liftM(F f, const _M<T> &m) {
        return monad<M>::bind(
                m, [f](const T &t) { return monad<_M<U>>::pure(f(t)); });
    }

    template <typename F, typename U = ResultOf<F(T)>>
    static constexpr _M<U> liftM(F f, _M<T> &&m) {
        return monad<M>::bind(std::move(m), [f](T &&t) {
            return monad<_M<U>>::pure(f(std::move(t)));
        });
    }
};

/**
 * Default monad with default "pure", "bind", "join", "ap" and "liftM".
 */
template <typename M>
struct default_monad : default_pure<M>,
                       default_bind<M>,
                       default_join<M>,
                       default_ap<M>,
                       default_liftM<M> {
    static constexpr bool instance = true;
};
}

#endif /* __ALGEBRA_CONTROL_MONAD_HPP__ */
