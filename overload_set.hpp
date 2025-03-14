#pragma once

#ifndef CPP_17
template <typename...> struct overload_set;

template <typename F1>
struct overload_set<F1> : public F1 {
    overload_set(F1&& f1) : F1(std::move(f1)) {}
    overload_set(const F1& f1) : F1(f1) {}
    using F1::operator();
};

template <typename F1, typename... Fs>
struct overload_set<F1, Fs...>
    : public F1, public overload_set<Fs...> {
    overload_set(F1&& f1, Fs&&... fs)
        : F1(std::move(f1))
        , overload_set<Fs...>(std::forward<Fs>(fs)...) {}
    overload_set(const F1& f1, Fs&&... fs)
        : F1(f1)
        , overload_set<Fs...>(std::forward<Fs>(fs)...) {}
    using F1::operator();
    using overload_set<Fs...>::operator();
};

#else

template <typename... Fs>
struct overload_set : public Fs... {
    overload_set(Fs&&... fs) : Fs(std::forward<Fs>(fs)...) {}
    using Fs::operator()...;
};

#endif

template <typename... Fs>
auto overload(Fs&&... fs) {
    return overload_set<Fs...>(std::forward<Fs>(fs)...);
}
