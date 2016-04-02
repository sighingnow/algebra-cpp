/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_BASIC_TYPE_CONCEPTS_HH__
#define __ALGEBRA_BASIC_TYPE_CONCEPTS_HH__

#include <type_traits>

/**
 * Basic concepts (check for various basic concepts).
 */
namespace algebra {

/**
 * Checks if a type has a constructor for specific arguments.
 */
template <typename T, typename... Args>
struct Constructible {
    static constexpr bool value = std::is_constructible<T, Args...>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a default constructor.
 */
template <typename T>
struct DefaultConstructible {
    static constexpr bool value = std::is_default_constructible<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a copy constructor.
 */
template <typename T>
struct CopyConstructible {
    static constexpr bool value = std::is_copy_constructible<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a move constructor.
 */
template <typename T>
struct MoveConstructible {
    static constexpr bool value = std::is_move_constructible<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a assignment operator for a specific argument.
 */
template <typename T, typename U>
struct Assignable {
    static constexpr bool value = std::is_assignable<T, U>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a copy assignment operator.
 */
template <typename T>
struct CopyAssignable {
    static constexpr bool value = std::is_copy_assignable<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a move assignment operator.
 */
template <typename T>
struct MoveAssignable {
    static constexpr bool value = std::is_move_assignable<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a non-deleted destructor.
 */
template <typename T>
struct Destructible {
    static constexpr bool value = std::is_destructible<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Checks if a type has a virtual destructor.
 */
template <typename T>
struct VirtualDestructor {
    static constexpr bool value = std::has_virtual_destructor<T>::value;
    constexpr operator bool() const noexcept { return value; }
};

/**
 * Check if a type is the base template of another parametrised type.
 *
 * e.g.:
 *      BaseTemplate<std::vector<int>, std::vector> = true
 *      BaseTemplate<std::vector<int>, std::map> = false
 */
template <typename T, template <typename...> class U>
struct BaseTemplate {
    static constexpr bool value = false;
};

template <template <typename...> class U, typename... Ts>
struct BaseTemplate<U<Ts...>, U> {
    static constexpr bool value = true;
};

/**
 * Check if two parametrised types are have the same base template.
 *
 * e.g.:
 *      SameTemplate<std::vector<int>, std::vector<float>> = true
 *      SameTemplate<std::vector<int>, std::list<int>> = false
 */
template <typename T, typename U>
struct SameTemplate;

template <template <typename...> class T, template <typename...> class U,
          typename... Ts,
          typename... Us  // Here, `Us` is just a placeholder, indicates that
                          // `U` is a parameterised
                          // tempalte.
          >
struct SameTemplate<T<Ts...>, U<Us...>> {
    static constexpr bool value = std::is_same<T<Ts...>, U<Ts...>>::value;
};

/**
 * Custom type concepts checker.
 */
template <bool Pred>
using Requires = typename std::enable_if<Pred>::type;
}

#endif /* __ALGEBRA_BASIC_TYPE_CONCEPTS_HH__ */
