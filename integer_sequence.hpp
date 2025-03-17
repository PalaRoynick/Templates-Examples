#pragma once

#include <cstddef>

// an empty simpliest struct with value_type and size() member
template <typename T, T... Integers>
struct integer_sequence {
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Integers); }
};

// each time we increment the I index, add it to the end of list of Integers
template <typename T, T I, T N, T... Integers>
struct make_integer_sequence_helper {
    using type = make_integer_sequence_helper<T, I + 1, N, Integers..., I>::type;
};

// partial specialization to finish recursion when we reach N
template <typename T, T N, T... Integers>
struct make_integer_sequence_helper<T, N, N, Integers...> {
    using type = integer_sequence<T, Integers...>;
};

template <typename T, T N> // T is assumed to be of an integer type
using make_integer_sequence = typename make_integer_sequence_helpe<r<T, 0, N>::type;

// e.g., N = 2 => <T, 0, 2> -> <T, 1, 2, 0> -> <T, 2, 2, 0, 1> -> integer_sequence<T, 0, 1>


// very important templates for T = std::size_t
template <std::size_t... Inds>
using index_sequence = integer_sequence<std::size_t, Inds...>;

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;
