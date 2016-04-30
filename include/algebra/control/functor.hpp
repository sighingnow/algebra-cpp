/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_H_DATA_FUNCTOR_HH__
#define __ALGEBRA_H_DATA_FUNCTOR_HH__

#include <algorithm>
#include <functional>
#include "../basic/type_concepts.hpp"
#include "../basic/type_operation.hpp"

/**
 * Functor: functors are mappings from one category to another. Map a function
 * on the real plain type value inner the context.
 * e.g., fmap fn vector<T> is `std::transform`.
 *
 * Functor laws:
 *  + identity: fmap id = id
 *  + composition: fmap (f . g)  =  fmap f . fmap g
 */
namespace algebra {

    template <typename F>
    struct functor {
        using T = ValueType<F>;

        template <typename U>
        using _F = Rebind<F, U>;

        /**
         * Minimal complete definition.
         */

        // Map a function to the inner value in a functor.
        // In Haskell:
        //      fmap :: (a -> b) -> f a -> f b
        template <typename Fn, typename U = ResultOf<Fn(T)>>
        static _F<U> fmap(Fn&& fn, const _F<T>& f);

        template <typename Fn, typename U = ResultOf<Fn(T)>>
        static _F<U> fmap(Fn&& fn, _F<T>&& f);

        // Just a generic class.
        static constexpr bool instance = false;
    };

    /**
     * Functor type predication.
     */
    template <typename F>
    struct Functor {
        static constexpr bool value = functor<F>::instance;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Operator overloading for functor operation.
     */

    // Overload `%` as `fmap`.
    // e.g.:
    //  auto fn = [](int x) { return x + 1; };
    //  std::vector<int> l = {1, 2, 3};
    //  auto res = fn % l; // equalize to `fmap(fn, l)`

    // For use ordinary function and lambda expression as `Fn`.
    template <
            typename Fn, typename F, typename _F = PlainType<F>,
            typename = Requires<Functor<_F>::value && !std::is_member_function_pointer<Fn>::value>>
    auto operator%(Fn&& fn, F&& f)
            -> decltype(functor<_F>::fmap(std::forward<Fn>(fn), std::forward<F>(f))) {
        return functor<_F>::fmap(std::forward<Fn>(fn), std::forward<F>(f));
    }

    // For use member function pointer as `Fn`.
    template <
            typename R, typename Fn, typename F, typename _F = PlainType<F>,
            typename = Requires<Functor<_F>::value && !std::is_member_function_pointer<Fn>::value>>
    auto operator%(R (Fn::*fn)(), F&& f)
            -> decltype(functor<_F>::fmap(std::mem_fn(fn), std::forward<F>(f))) {
        return functor<_F>::fmap(std::mem_fn(fn), std::forward<F>(f));
    }

    // For use transformed lambda expression as `Fn`.
    template <
            typename R, typename Fn, typename F, typename _F = PlainType<F>,
            typename = Requires<Functor<_F>::value && !std::is_member_function_pointer<Fn>::value>>
    auto operator%(R (Fn::*fn)() const, F&& f)
            -> decltype(functor<_F>::fmap(std::mem_fn(fn), std::forward<F>(f))) {
        return functor<_F>::fmap(std::mem_fn(fn), std::forward<F>(f));
    }
};

#endif /* __ALGEBRA_H_DATA_FUNCTOR_HH__ */
