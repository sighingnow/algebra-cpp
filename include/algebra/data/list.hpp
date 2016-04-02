/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#ifndef __ALGEBRA_H_DATA_LIST_HPP__
#define __ALGEBRA_H_DATA_LIST_HPP__

#include <list>
#include "../control/functor.hpp"
#include "../data/monoid.hpp"

/**
 * List: monoid, functor, applicative, monad.
 */
namespace algebra {

/**
 * List as monoid.
 */
template <typename... Ts>
struct monoid<std::list<Ts...>> {
    static std::list<Ts...> mempty() { return std::list<Ts...>{}; }

    static std::list<Ts...> mappend(const std::list<Ts...>& l1,
                                    const std::list<Ts...>& l2) {
        std::list<Ts...> t = l1;
        t.insert(std::end(t), std::begin(l2), std::end(l2));
        return t;
    }

    static std::list<Ts...> mappend(std::list<Ts...>&& l1,
                                    const std::list<Ts...>& l2) {
        l1.insert(std::end(l1), std::begin(l2), std::end(l2));
        return std::move(l1);
    }

    static std::list<Ts...> mappend(const std::list<Ts...>& l1,
                                    std::list<Ts...>&& l2) {
        l2.insert(std::begin(l2), std::begin(l1), std::end(l1));
        return std::move(l2);
    }

    static std::list<Ts...> mappend(std::list<Ts...>&& l1,
                                    std::list<Ts...>&& l2) {
        std::move(std::begin(l2), std::end(l2), std::back_inserter(l1));
        return std::move(l1);
    }

    static constexpr bool instance = true;
};

/**
 * List as functor.
 */
template <typename T, typename... Ts>
struct functor<std::list<T, Ts...>> {
    template <typename Fn, typename U = ResultOf<Fn(T)>>
    static std::list<U> fmap(Fn&& fn, const std::list<T>& l) {
        std::list<U> result;
        for (auto& e : l) {
            result.emplace_back(fn(e));
        }
        return result;
    }

    template <typename Fn, typename U = ResultOf<Fn(T)>,
              typename = Requires<!std::is_same<U, T>::value ||
                                  (!std::is_copy_assignable<T>::value &&
                                   !std::is_move_assignable<T>::value)>>
    static std::list<U> fmap(Fn&& fn, std::list<T>&& l) {
        std::list<U> result;
        for (auto& e : l) {
            result.emplace_back(fn(std::move(e)));
        }
        return result;
    }

    // In-place fmap.
    template <typename Fn, typename U = ResultOf<Fn(T)>,
              typename = Requires<std::is_same<U, T>::value &&
                                  (std::is_copy_assignable<T>::value ||
                                   std::is_move_assignable<T>::value)>>
    static std::list<T> fmap(Fn&& fn, std::list<T>&& l) {
        std::transform(std::begin(l), std::end(l), std::begin(l), fn);
        return l;
    }

    static constexpr bool instance = true;
};

/**
 * List as monad.
 */

// TODO
};

#endif /* __ALGEBRA_H_DATA_LIST_HPP__ */
