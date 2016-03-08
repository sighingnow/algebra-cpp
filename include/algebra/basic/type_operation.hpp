/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_TYPE_OPERATION_HH__
#define __ALGEBRA_TYPE_OPERATION_HH__

/**
 * Basic higher-order combinator to manipulate types.
 */
namespace algebra {

    /**
     * Find the first-level contained type in template type parameters.
     */

    // For ordinary parametrised templates, e.g., std::vector<int>
    template<typename T>
    struct InnerType {
        using type = T;
    };

    // For nested parametrised templates, e.g., std::vector<std::vector<int>>
    template<template<typename> class Tt, typename T>
    struct InnerType<Tt<T>> {
        using type = T;
    };

    // For multiple levels nested parametrised templates.
    template<template<typename, typename...> class Tt, typename T, typename... Ts>
    struct InnerType<Tt<T, Ts...>> {
        using type = T;
    };

    /**
     * Bind parametric tempalte's type parameter with given type value.
     */

    namespace _inner_impl {
        // For ordinary parametrised templates.
        template<typename T, typename>
        struct rebind {
            using type = T;
        };

        // For nested parametrised templates.
        template<template<typename> class Tt, typename T, typename U>
        struct rebind<Tt<T>, U> {
            using type = Tt<U>;
        };

        // For multiple levels nested parametrised templates.
        template<template<typename...> class Tt, typename T, typename U, typename... Ts>
        struct rebind<Tt<T, Ts...>, U> {
            using type = Tt<U, Ts...>;
        };
    }

    template<typename T>
    struct parametric_type_traits {
        // Get the value of type parameter.
        using value_type = typename InnerType<T>::type;
        // Bind type parameter with concrete type value.
        template<typename U>
        using rebind = typename _inner_impl::rebind<T, U>::type;
    };

    template<typename T>
    struct parametric_type_traits<const T> {
        using value_type = typename parametric_type_traits<T>::value;
        template<typename U>
        using rebind = const typename parametric_type_traits<T>::template rebind<U>;
    };

    // Export more concise interface.

    template<typename T>
    using ValueType = typename parametric_type_traits<T>::value_type;

    template<typename T, typename U>
    using Rebind = typename parametric_type_traits<T>::template rebind<U>;

    /**
     * Get the return type of a function.
     */

    template<typename T>
    using PlainType = typename std::decay<T>::type;

    namespace _inner_impl {
        template<typename>
        struct decayed_result;

        template<typename F, typename... Ts>
        struct decayed_result<F(Ts...)> {
            using type = PlainType<typename std::result_of<F(Ts...)>::type>;
        };
    }

    template<typename F>
    using ResultOf = typename _inner_impl::decayed_result<F>::type;
}

#endif /* __ALGEBRA_TYPE_OPERATION_HH__ */


