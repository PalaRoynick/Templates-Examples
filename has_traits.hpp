#pragma once

#include <utility>
#include <type_traits>

/*
    first we write a class to convert from two types into a bool
    depending on the '<' operation availability
*/

// 0. two template parameters
template <typename T1, typename T2>
class has_less {
    // 4. helper
    template <typename T> struct Helper;
    // 3. the main case: we need some raw pointer which wraps the check for '<'
    //    we use some helper templated structure
    //    we parametrize it with the type of expression '<'
    template <typename U1, typename U2>
    static std::true_type test(Helper<decltype(std::declval<U1>() < std::declval<U2>())> *);

    // 2. fall back for failure case: no need for body, use under decltype
    template <typename U1, typename U2>
    static std::false_type test(...);
public:

    // 1. we need an access to the value underlying via std::true_type or std::false_type ::value
    //    delegate to the private 'test' method to resolve
    //
    //    static constexpr bool value = decltype(???)::value;
    //
    //    ??? <- test<T1, T2>(nullptr): the 'test' is parametrized with T1, T2 and takes some dummy pointer
    //
    static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

/*
    Next, we need a type trait to use in SFINAE contexts, e.g. under enable_if
*/

// 3. add implementation struct: primary template
template <typename T1, typename T2, bool HasLess>
struct has_less_type_impl {
    // repeat some code here because now we need info for type, not for value!
    // note there is no substitution failure here
    using Type = decltype(std::declval<T1>() < std::declval<T2>());
};

// 3. specialization for false case
template <typename T1, typename T2>
struct has_less_type_impl<T1, T2, false> { /*empty*/ };

// 2. the holder for type:
template <typename T1, typename T2>
struct has_less_type_holder
 : has_less_type_impl<T1, T2, has_less<T1, T2>::value> {
    /*delegate to the struct with partial specialization*/
    // SFINAE is used here, in substitution of has_less.
    // then just simple partial specialization resolution.
    // in a case of no '<', no Type 

    // ?? can we use alias like this here?
    //using Type = has_less_type_impl<T1, T2, has_less<T1, T2>::value>::Type;
};

// 1. we want alias for type which we finally use in the main code:
//    delegate to some implementation which wraps the underlying type
//    *don't forget typename keyword!*
template <typename T1, typename T2>
using has_less_type = typename has_less_type_holder<T1, T2>::Type; // in a case of no '<', ::Type access is an error!

/*
    Finally, how we do use it

    e.g., implement a minimum function
*/

#if 0
template <typename T,
          typename = std::enable_if_t<std::is_convertible<has_less_type<const T&, const T&>, bool>::value, T>>
const T& min(const T& a, const T& b) {
    if (a < b)
        return a;
    return b;
}
#endif

// The problem of the above min function is a dummy template parameter for dispatching.
// Let's rewrite it without a dummy template parameter
template <typename T,
          std::enable_if_t<std::is_convertible<has_less_type<const T&, const T&>, bool>::value, int> = 0>
const T& min(const T& a, const T& b) {
    if (a < b)
        return a;
    return b;
}
