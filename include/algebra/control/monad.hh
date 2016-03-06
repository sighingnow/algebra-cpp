/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_CONTROL_MONAD_HH__
#define __ALGEBRA_CONTROL_MONAD_HH__

#include "../basic/type_operation.hh"

/**
 * Monad: represents sequenceable computations in a context.
 */
namespace algebra {
    template<typename _M>
    struct monad {
        // Just a generic type class, not monad instance.
        static constexpr bool instance = false;

        // Provide type signatures for this specialised monad.
        template<typename U>
        using M = Rebind<_M, U>;

        // The inner value type in the type parameter `_M`.
        using T = ValueType<_M>;

        /**
         * Minimal complete definition.
         */

        static M<T> pure(const T&);

        template<typename F, typename U = ValueType<ResultOf<F(T)>>>
        static M<U> bind(const M<T>& m, F&& f);

        /**
         * Other useful methods.
         */

        static M<T> join(const M<M<T>>& m);

        template<typename F, typename U = ResultOf<F(T)>>
        static M<U> apply(const M<F>& fn, const M<T>& m);

        template<typename F, typename U = ResultOf<F(T)>>
        static M<U> map(F&& fn, const M<T>& m);
    };

    /**
     * Monad type predication.
     */
    template<typename M>
    struct Monad {
        static constexpr bool value = monad<M>::instance;
        constexpr operator bool () const noexcept {
            return value;
        }
    };
}

#endif /* __ALGEBRA_CONTROL_MONAD_HH__ */


