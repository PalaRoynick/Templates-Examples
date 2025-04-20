#include <type_traits>

template <std::size_t N>
struct fibo : std::integral_constant<std::size_t,
                fibo<N - 1>{} + fibo<N - 2>{}> {};

template <>
struct fibo<0> : std::integral_constant<std::size_t, 1> {};

template <>
struct fibo<1> : std::integral_constant<std::size_t, 1> {};

int main() {
    static_assert(fibo<5>::value == 8);
    static_assert(fibo<7>::value == 21);
}