#include <type_traits>
#include <iostream>

/* imperative style */
int mySqrt(int x) {
    int l = 1, r = x;
    while (l < r) {
        int m = (l + r) / 2;
        if (m > x / m)
            r = m;
        else
            l = m + 1;
    }
    return l - 1;
}

/* functional programming style */
int isqrt_rt(int N, int lo, int hi) {
    if (N == 0) return 0;
    if (N == 1) return 1;
    if (N == 2) return 1;

    // this looks like a specialization to 'close' recursion
    if (lo == hi)
        return lo - 1;

    int mid = (lo + hi) / 2;

    // this if-else looks like std::conditional_t
    if (mid > N / mid)
        return isqrt_rt(N, lo, mid);
    else
        return isqrt_rt(N, mid + 1, hi);
}

template <int N, int lo = 1, int hi = N - 1>
struct isqrt : std::conditional_t<
                ((lo + hi)/2 > N / ((lo + hi)/2)),
                typename std::integral_constant< int, isqrt<N, lo, (lo + hi)/2>{} >,
                typename std::integral_constant< int, isqrt<N, (lo + hi)/2 + 1, hi >{} >
               >
{};

template <int lo, int hi>
struct isqrt<0, lo, hi> : std::integral_constant<int, 0> {};

template <int lo, int hi>
struct isqrt<1, lo, hi> : std::integral_constant<int, 1> {};

template <int lo, int hi>
struct isqrt<2, lo, hi> : std::integral_constant<int, 1> {};

template <int N, int lo>
struct isqrt<N, lo, lo> : std::integral_constant<int, lo - 1> {};

int main() {
    int x;
    std::cin >> x;
    std::cout << isqrt_rt(x, 1, x - 1) << std::endl;

    static_assert(isqrt<16>::value == 4);
    static_assert(isqrt<101>::value == 10);
    // static_assert(isqrt<1234567>::value == 1111);
}