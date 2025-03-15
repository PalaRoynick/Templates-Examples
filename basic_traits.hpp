#pragma once

/*
    This file contains the most famous type traits in STL,
    but made by hand to train the basics.
*/

namespace my_traits { // to eliminate name collisions
    // I. conditional traits

//////////////// ternary operator for types: conditional ////////////////

// primary template
template <bool B, typename T, typename U>
struct conditional {
    using Type = T;
};

// partial specializtion for false
template <typename T, typename U>
struct conditional<false, T, U> {
    using Type = U;
};

// alias
template <bool B, typename T, typename U>
using conditional_t = typename conditional<B, T, U>::Type;

//////////////// transform from {true, false} -> {valid, invalid}: ////////////////
////////////////                 enable_if                         ////////////////

// just erase on of types: U from above

template <bool B, typename T = void>
struct enable_if {
    using Type = T;
};

template <typename T>
struct enable_if<false, T> {
    /* empty */
};

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::Type;

} // my_traits

/*
    Next we consider some interesting use cases
*/

// 1. we want dispatch for types depending on their sizes

template <typename T, typename = my_traits::enable_if_t<(sizeof(T) < 4)>>
void foo(T x) { /* ... */ }

//    how we add the opposite case?

//    ok, try this:

#if 0
template <typename T, typename = my_traits::enable_if_t<(sizeof(T) >= 4)>>
void foo(T x) { /* ... */ } // error in name resolution! (ODR violation)
#endif

//    ok, a dummy function parameter solves this, but looks ugly...
template <typename T, typename = my_traits::enable_if_t<(sizeof(T) >= 4)>>
void foo(T x, int dummy = 0) {}

//    a better approach is not to use an unnamed template parameter, but make it an explicit one.
//    and this new parameter is wrapped by enable_if
template <typename T, my_traits::enable_if_t<(sizeof(T) >= 4)>>
void foo(T x) {}

//    if we just leave it as is, we have to explicitly write this template parameter each time
template <typename T, my_traits::enable_if_t<(sizeof(T) >= 4)>* = nullptr>
void foo(T x) {}

//    a pointer is a bit scary
template <typename T, my_traits::enable_if_t<(sizeof(T) >= 4), int> = 0>
void foo(T x) {}