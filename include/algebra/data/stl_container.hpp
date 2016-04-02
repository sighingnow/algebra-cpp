/**
 * The MIT License(MIT). Copyright (c) 2016 He Tao
 */

#ifndef __ALGEBRA_H_DATA_LIST_HPP__
#define __ALGEBRA_H_DATA_LIST_HPP__

#include <list>
#include <string>
#include <vector>
#include "../control/functor.hpp"
#include "../control/monad.hpp"
#include "../data/monoid.hpp"

/**
 * Instantiate STL container as functional type classes.
 */
namespace algebra {

/**
 * Tag used for STL containers.
 */
template <typename...>
struct stl_container {};

// Specialize `Rebind` for Value-Allocator containers: generic collection with
// type `C <T, Allocator>`.
template <template <typename, typename> class C, typename T, typename A>
struct parametric_type_traits<C<T, A>> {
   private:
    template <typename U>
    using rebind_allocator =
            typename std::allocator_traits<A>::template rebind_alloc<U>;

   public:
    using value_type = T;

    template <typename U>
    using rebind = C<U, rebind_allocator<U>>;
};

/**
 * STL containers as monoid.
 */
template <typename M>
struct monoid<stl_container<M>> {
    static M mempty() { return M{}; }

    static M mappend(const M& l1, const M& l2) {
        M t = l1;
        t.insert(std::end(t), std::begin(l2), std::end(l2));
        return t;
    }

    static M mappend(M&& l1, const M& l2) {
        l1.insert(std::end(l1), std::begin(l2), std::end(l2));
        return std::move(l1);
    }

    static M mappend(const M& l1, M&& l2) {
        l2.insert(std::begin(l2), std::begin(l1), std::end(l1));
        return std::move(l2);
    }

    static M mappend(M&& l1, M&& l2) {
        std::move(std::begin(l2), std::end(l2), std::back_inserter(l1));
        return std::move(l1);
    }

    static constexpr bool instance = true;
};

/**
 * STL containers as functor.
 */
template <typename F>
struct functor<stl_container<F>> {
    using T = ValueType<F>;

    template <typename U>
    using _F = Rebind<F, U>;

    template <typename Fn, typename U = ResultOf<Fn(T)>,
              typename = Requires<!std::is_same<U, T>::value ||
                                  (!std::is_copy_assignable<T>::value &&
                                   !std::is_move_assignable<T>::value)>>
    static _F<U> fmap(Fn&& fn, const _F<T>& container) {
        _F<U> result;
        for (auto& e : container) {
            result.emplace_back(fn(std::move(e)));
        }
        return result;
    }

    template <typename Fn, typename U = ResultOf<Fn(T)>,
              typename = Requires<!std::is_same<U, T>::value ||
                                  (!std::is_copy_assignable<T>::value &&
                                   !std::is_move_assignable<T>::value)>>
    static _F<U> fmap(Fn&& fn, _F<T>&& container) {
        _F<U> result;
        for (auto& e : container) {
            result.emplace_back(fn(std::move(e)));
        }
        return result;
    }

    // In place fmap.
    template <typename Fn, typename U = ResultOf<Fn(T)>,
              typename = Requires<std::is_same<U, T>::value &&
                                  (std::is_copy_assignable<T>::value ||
                                   std::is_move_assignable<T>::value)>>
    static _F<T> fmap(Fn&& fn, _F<T>&& container) {
        // for (auto &e :container) {
        //     e = fn(std::move(e));
        // }
        std::transform(std::begin(container), std::end(container),
                       std::begin(container), fn);
        return container;
    }

    static constexpr bool instance = true;
};

/**
 * STL containers as monad.
 */
template <typename M>
struct monad<stl_container<M>> : default_monad<M> {
    using T = ValueType<M>;

    template <typename U>
    using _M = Rebind<M, U>;

    template <typename F, typename R = ResultOf<F(T)>,
              typename U = ValueType<R>,
              typename = Requires<DefaultConstructible<_M<U>>::value>>
    static _M<U> bind(const _M<T>& m, F&& f) {
        auto r = std::forward<F>(f) % m;
        _M<U> result;
        auto it = std::inserter(result, std::begin(result));
        for (auto& e : r) {
            it = std::move(std::begin(e), std::end(e), it);
        }
        return result;
    }

    template <typename F, typename R = ResultOf<F(T)>,
              typename U = ValueType<R>,
              typename = Requires<DefaultConstructible<_M<U>>::value>>
    static _M<U> bind(_M<T>&& m, F&& f) {
        auto r = std::forward<F>(f) % std::move(m);
        _M<U> result;
        auto it = std::inserter(result, std::begin(result));
        for (auto& e : r) {
            it = std::move(std::begin(e), std::end(e), it);
        }
        return result;
    }
};

/**
 * For `std::list`.
 */

template <typename T, typename A>
struct monoid<std::list<T, A>> : monoid<stl_container<std::list<T, A>>> {};
template <typename T, typename A>
struct functor<std::list<T, A>> : functor<stl_container<std::list<T, A>>> {};
template <typename T, typename A>
struct monad<std::list<T, A>> : monad<stl_container<std::list<T, A>>> {};

/**
 * For `std::vector`.
 */

template <typename T, typename A>
struct monoid<std::vector<T, A>> : monoid<stl_container<std::vector<T, A>>> {};
template <typename T, typename A>
struct functor<std::vector<T, A>> : functor<stl_container<std::vector<T, A>>> {
};
template <typename T, typename A>
struct monad<std::vector<T, A>> : monad<stl_container<std::vector<T, A>>> {};

/**
 * For `std::basic_string`
 */
template <typename... Ts>
struct monoid<std::basic_string<Ts...>>
        : monoid<stl_container<std::basic_string<Ts...>>> {};
};

#endif /* __ALGEBRA_H_DATA_LIST_HPP__ */
