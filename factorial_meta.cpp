#include <type_traits>

template <std::size_t N>
struct factorial : std::integral_constant<std::size_t, N * factorial<N - 1>::value> {};

template <>
struct factorial<0> : std::integral_constant<std::size_t, 1> {};

int main() {
    static_assert(factorial<5>::value == 120);
}