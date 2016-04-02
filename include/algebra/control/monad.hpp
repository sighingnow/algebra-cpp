/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_CONTROL_MONAD_HPP__
#define __ALGEBRA_CONTROL_MONAD_HPP__

#include "../basic/type_concepts.hpp"
#include "../basic/type_operation.hpp"
#include "../control/applicative.hpp"
#include "../prelude.hpp"

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
    // Provide type signatures for this specialised monad.
    template <typename U>
    using _M = Rebind<M, U>;

    // The inner value type in the type parameter `_M`.
    using T = ValueType<M>;

    /**
     * Minimal complete definition.
     */

    // bind: monad m => m a -> (a -> m b) -> m b.
    template <typename F, typename U = ValueType<ResultOf<F(T)>>>
    static _M<U> bind(const _M<T> &m, F &&f);

    /**
     * Other useful methods.
     */

    // pure: monad m => a -> m a.
    static _M<T> pure(const T &);

    // join: monad m => m (m a) => m a.
    static _M<T> join(const _M<_M<T>> &m);

    // ap (apply): monad m => m (a -> b) -> m a -> m b.
    template <typename F, typename U = ResultOf<F(T)>>
    static _M<U> ap(const _M<F> &fn, const _M<T> &m);

    // liftM: monad m => (a -> b) -> m a -> m b.
    template <typename F, typename U = ResultOf<F(T)>>
    static _M<U> liftM(F &&fn, const _M<T> &m);

    // Just a generic type class, not monad instance.
    static constexpr bool instance = false;
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
 * Default definition for `pure`, `bind`, `map` and `join`.
 */

// `default_pure` is already defined in `../control/applicative.hpp`.

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

// In haskell:
//      join: monad m => m (m a) => m a.
//      join x = x >>= id
//
// The function `join` is used to remove one level of monadic structure. Here, I
// make the type of `join` as `monad m => m a -> a` rather than the origin type
// signature in Haskell.
template <typename M>
struct default_join {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    static constexpr _M<T> join(const _M<_M<T>> &m) {
        return monad<_M<_M<T>>>::bind(m, _id);
    }

    static constexpr _M<T> join(_M<_M<T>> &&m) {
        return monad<_M<_M<T>>>::bind(std::move(m), _id);
    }
};

// ap (apply): monad m => m (a -> b) -> m a -> m b.
// In haskell:
//      ap m1 m2 = do { x1 <- m1; x2 <- m2; return (x1 x2) }
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
// In haskell:
//      liftM f m1 = do { x1 <- m1; return (f x1) }
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

/**
 * Operator overlaoding for monad operation.
 */

// Use `>>=` to represent `bind`.
//  a >>= b = bind(a, b).

// for ordinary function and ordinary function pointer.
template <typename M, typename F, typename _M = PlainType<M>,
          typename = Requires<Monad<_M>{} &&
                              !std::is_member_function_pointer<F>::value>>
auto operator>>=(M &&m, F &&f)
        -> decltype(monad<_M>::bind(std::move(m), std::forward<F>(f))) {
    return monad<_M>::bind(std::move(m), std::forward<F>(f));
}

// for lambda expression.
template <typename M, typename F, typename _M = PlainType<M>,
          typename = Requires<Monad<_M>{}>>
auto operator>>=(M &&m, F (M::*f)() const)
        -> decltype(monad<M>::bind(std::move(m), std::forward<F>(f))) {
    return std::move<M>(m) >>= std::mem_fn<F>(f);
}

// for functor object (callable struct, struct with overloaded `()` operator).
template <typename M, typename F, typename _M = PlainType<M>,
          typename = Requires<Monad<_M>{}>>
auto operator>>=(M &&m, F (M::*f)())
        -> decltype(monad<M>::bind(std::move(m), std::forward<F>(f))) {
    return std::move<M>(m) >>= std::mem_fn<F>(f);
}

// Use `<<=` to represent reverse bind.
template <typename M, typename F, typename _M = PlainType<M>,
          typename = Requires<Monad<_M>{}>>
auto operator<<=(F &&f, M &&m)
        -> decltype(std::move<M>(m) >>= std::forward<F>(f)) {
    return std::move<M>(m) >>= std::forward<F>(f);
}

// Use `>>` to represent the bind with discard the first result. The two monadic
// compuatation will be all performed.
template <typename MA, typename MB, typename _MA = PlainType<MA>,
          typename _MB = PlainType<MB>,
          typename = Requires<Monad<_MA>{} && Monad<_MB>{}>>
_MB operator>>(MA &&, MB &&) {
    // TODO
    // return std::forward<MA>(ma) >>= _const(std::forward<MB>(mb));
}
}

#endif /* __ALGEBRA_CONTROL_MONAD_HPP__ */
