/**
 * The MIT License(MIT). Copyright (c) 2015-2016 He Tao
 */

#ifndef __ALGEBRA_BASIC_TYPE_CONCEPTS_HPP__
#define __ALGEBRA_BASIC_TYPE_CONCEPTS_HPP__

#include <iterator>
#include "../basic/type_operation.hpp"

/**
 * Concepts can be used to specify the requirements on template arguments and to
 * select the most appropriate function overloads and template specializations.
 *
 * The extension of concepts will not be C++17 standard, so I implements
 * equivalent facilities and my own `Requires` using `std::enable_if` to put
 * constraints on type parameters of templates.
 *
 * The proposal <Programming Languages - C++ Extensions for Concepts> contains
 * the detail of concepts. See the document at:
 *      http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4377.pdf
 *
 * There are also detailed reference for C++ concepts from cppreference.com, at:
 *      http://en.cppreference.com/w/cpp/concept
 */
namespace algebra {

    /**
     * Custom type concepts checker.
     */
    template <bool Pred>
    using Requires = typename std::enable_if<Pred>::type;

    /**
     * Basic concepts.
     *
     *  + DefaultConstructible.
     *  + MoveConstructible.
     *  + CopyConstructible.
     *  + MoveAssignable.
     *  + CopyAssignable.
     *  + Assignable.
     *  + Constructible.
     *  + Destructible.
     *  + VirtualDestructible.
     */

    /**
     * Specifies that an object of the type can be default constructed.
     */
    template <typename T>
    struct DefaultConstructible {
        static constexpr bool value = std::is_default_constructible<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be constructed from rvalue.
     */
    template <typename T>
    struct MoveConstructible {
        static constexpr bool value = std::is_move_constructible<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be constructed from lvalue.
     */
    template <typename T>
    struct CopyConstructible {
        static constexpr bool value = std::is_copy_constructible<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be assigned from rvalue.
     */
    template <typename T>
    struct MoveAssignable {
        static constexpr bool value = std::is_move_assignable<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be assigned from lvalue.
     */
    template <typename T>
    struct CopyAssignable {
        static constexpr bool value = std::is_copy_assignable<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be assigned from value with the
     * given specific data type.
     */
    template <typename T, typename U>
    struct Assignable {
        static constexpr bool value = std::is_assignable<T, U>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be constructed using arguments with
     * the given specific data types.
     */
    template <typename T, typename... Args>
    struct Constructible {
        static constexpr bool value = std::is_constructible<T, Args...>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type can be destroyed.
     */
    template <typename T>
    struct Destructible {
        static constexpr bool value = std::is_destructible<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies that an object of the type has a virtual destructor.
     */
    template <typename T>
    struct VirtualDestructible {
        static constexpr bool value = std::has_virtual_destructor<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Layout:
     *
     *  + TriviallyCopyable.
     *  + TrivialType.
     *  + StandardLayoutType.
     *  + PODType.
     */

    /**
     * Specifies class with trivial copy, assignment and destructor.
     */
    template <typename T>
    struct TriviallyCopyable {
        static constexpr bool value = std::is_trivially_copyable<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies class with trivial constructors, assignment and destructor.
     */
    template <typename T>
    struct TrivialType {
        static constexpr bool value = std::is_trivial<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies non-virtual class containing only other StandardLayout members, all
     * with the same access control.
     */
    template <typename T>
    struct StandardLayoutType {
        static constexpr bool value = std::is_standard_layout<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Specifies POD (Plain Old Data) structure, compatible with C struct.
     */
    template <typename T>
    struct PODType {
        static constexpr bool value = std::is_pod<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Library-wide
     *
     *  + Boolean
     *  + EuqalityComparable.
     *  + LessThanComparable.
     *  + Swappable.
     *  + ValueSwappable.
     *  + NullablePointer.
     *  + Hash.               TODO
     *  + Allocator.          TODO
     *  + FunctionObject.
     *  + Callable.
     *  + Predicate.          TODO
     *  + BinaryPredicate.    TODO
     *  + Compare.            TODO
     */

    /**
     * If a type T is contextually convertible to bool.
     */
    template <typename T>
    struct Boolean {
        static constexpr bool value =
                std::is_convertible<T, bool>::value;  // or std::is_constructible
        constexpr operator bool() const noexcept { return value; }
    };

    namespace _inner_impl {
        // Check `==` using SFINAE.
        template <typename A, typename B>
        auto test_eq(decltype(std::declval<A>() == std::declval<B>()) *)
                -> decltype(std::declval<A>() == std::declval<B>());
        template <typename A, typename B>
        auto test_eq(...) -> void;
    };

    /**
     * Operator== is an equivalence relation.
     */
    template <typename A, typename B>
    struct EqualityComparable {
        static constexpr bool value = Boolean<decltype(_inner_impl::test_eq<A, B>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename T>
    struct EqualityType {
        static constexpr bool value = Boolean<decltype(_inner_impl::test_eq<T, T>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    namespace _inner_impl {
        // Check `<` using SFINAE.
        template <typename A, typename B>
        auto test_lt(decltype(std::declval<A>() < std::declval<B>()) *)
                -> decltype(std::declval<A>() < std::declval<B>());
        template <typename A, typename B>
        auto test_lt(...) -> void;
    };

    /**
     * Operator< is a strict weak ordering relation.
     */
    template <typename A, typename B>
    struct LessThanComparable {
        static constexpr bool value = Boolean<decltype(_inner_impl::test_lt<A, B>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename T>
    struct LessThanType {
        static constexpr bool value = Boolean<decltype(_inner_impl::test_lt<T, T>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    namespace _inner_impl {
        // Check `std::swap` using SFINAE.
        template <typename T, typename U>
        auto test_lref_swap(decltype(std::true_type{}) *)
                -> decltype(std::swap(std::declval<LRef<T>>(), std::declval<LRef<U>>()),
                            std::true_type{});
        template <typename T, typename U>
        auto test_lref_swap(...) -> void;

        template <typename T, typename U>
        auto test_rref_swap(decltype(std::true_type{}) *)
                -> decltype(std::swap(std::declval<RRef<T>>(), std::declval<RRef<U>>()),
                            std::true_type{});
        template <typename T, typename U>
        auto test_rref_swap(...) -> void;
    };

    /**
     * Can be swapped with an unqualified non-member function call swap() .
     */
    template <typename T, typename U>
    struct Swappable {
        static constexpr bool value =
                Boolean<decltype(_inner_impl::test_lref_swap<T, U>(nullptr))>::value &&
                Boolean<decltype(_inner_impl::test_rref_swap<T, U>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * an Iterator that dereferences to a Swappable type.
     */
    template <typename ItT, typename ItU>
    struct ValueSwappable {
        static constexpr bool value =
                Swappable<typename std::iterator_traits<ItT>::value_type,
                          typename std::iterator_traits<ItU>::value_type>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    namespace _inner_impl {
        /**
         * If a type T can be assigned with nullptr.
         */
        template <typename T>
        struct is_nullptr_assignable {
           private:
            std::nullptr_t np = nullptr;
            const std::nullptr_t npc = nullptr;

           public:
            static constexpr bool value =
                    std::is_same<T &, decltype(std::declval<T>() =
                                                       std::declval<std::nullptr_t>())>::value &&
                    std::is_same<T &,
                                 decltype(std::declval<T>() =
                                                  std::declval<const std::nullptr_t>())>::value &&
                    Constructible<T, std::nullptr_t>::value &&
                    Constructible<T, const std::nullptr_t>::value;
            constexpr operator bool() const noexcept { return value; }
        };
    };

    /**
     * NullablePointer. See
     * http://en.cppreference.com/w/cpp/concept/NullablePointer.
     */
    template <typename T>
    struct NullablePointer {
        static constexpr bool value =
                EqualityComparable<T, std::nullptr_t>::value &&
                EqualityComparable<std::nullptr_t, T>::value && DefaultConstructible<T>::value &&
                CopyConstructible<T>::value && CopyAssignable<T>::value && Destructible<T>::value &&
                Boolean<T>::value && _inner_impl::is_nullptr_assignable<T>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * FunctionObject.
     */
    template <typename F>
    struct FunctionObject {
        static constexpr bool value = std::is_function<F>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
     * Callable.
     */
    namespace _inner_impl {
        /**
         * Check if F can be applied on Args using SFINAE.
         */
        template <typename F, typename... Args>
        auto test_invoke(decltype(std::true_type{}) *)
                -> decltype(std::declval<F>()(std::declval<Args>()...), std::true_type{});
        template <typename F, typename... Args>
        auto test_invoke(...) -> void;
    };

    template <typename F, typename... Args>
    struct Callable {
        static constexpr bool value =
                FunctionObject<F>::value &&
                Boolean<decltype(_inner_impl::test_invoke<F, Args...>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename F, typename T>
    struct Predicate {
        static constexpr bool value =
                Callable<F, T>::value && std::is_convertible<ResultOf<F(T)>, bool>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename F, typename T>
    struct BinaryPredicate {
        static constexpr bool value =
                Callable<F, T, T>::value && std::is_convertible<ResultOf<F(T, T)>, bool>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    namespace _inner_impl {
        // Check `<` using SFINAE.
        template <typename A, typename B>
        auto test_comp(decltype(comp(std::declval<A>(), std::declval<B>())) *)
                -> decltype(comp(std::declval<A>(), std::declval<B>()));
        template <typename A, typename B>
        auto test_comp(...) -> void;
    };

    /**
     * Operator< is a strict weak ordering relation.
     */
    template <typename T>
    struct Compare {
        static constexpr bool value =
                Boolean<decltype(_inner_impl::test_comp<T, T>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    /**
    * Containter
    *
    *  + Container.
    *  + ReversibleContainer.
    *  + AllocatorAwareContainer.
    *  + SquenceContainer.
    *  + ContiguousContainer.
    *  + AssociativeContainer.
    *  + UnorderedAssociativeContainer.
    *
    * TODO
    */

    /**
     * Iterator
     *
     *  + Iterator.
     *  + InputIterator.
     *  + OutputIterator.
     *  + ForwardIterator.
     *  + BidirectionalIterator.
     *  + RandomAccessIterator.
     */

    namespace _inner_impl {
        template <typename It>
        auto test_iterator(decltype(std::iterator_traits<It>::pointer) *)
                -> decltype(std::iterator_traits<It>::pointer, std::true_type{});
        template <typename It>
        void test_iterator(...);
    };

    template <typename It>
    struct Iterator {
        static constexpr bool value =
                Boolean<decltype(_inner_impl::test_iterator<It>(nullptr))>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename It>
    struct InputIterator {
        static constexpr bool value =
                std::is_same<std::input_iterator_tag,
                             typename std::iterator_traits<It>::iterator_category>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename It>
    struct OutputIterator {
        static constexpr bool value =
                std::is_same<std::output_iterator_tag,
                             typename std::iterator_traits<It>::iterator_category>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename It>
    struct ForwardIterator {
        static constexpr bool value =
                std::is_same<std::forward_iterator_tag,
                             typename std::iterator_traits<It>::iterator_category>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename It>
    struct BidirectionalIterator {
        static constexpr bool value =
                std::is_same<std::bidirectional_iterator_tag,
                             typename std::iterator_traits<It>::iterator_category>::value;
        constexpr operator bool() const noexcept { return value; }
    };

    template <typename It>
    struct RandomAccessIterator {
        static constexpr bool value =
                std::is_same<std::random_access_iterator_tag,
                             typename std::iterator_traits<It>::iterator_category>::value;
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

    template <template <typename...> class T, template <typename...> class U, typename... Ts,
              typename... Us  // Here, `Us` is just a placeholder, indicates that
                              // `U` is a parameterised
                              // tempalte.
              >
    struct SameTemplate<T<Ts...>, U<Us...>> {
        static constexpr bool value = std::is_same<T<Ts...>, U<Ts...>>::value;
    };
};

#endif /* __ALGEBRA_BASIC_TYPE_CONCEPTS_HPP__ */
