#pragma once

#include <iostream>
#include <vector>
#include <deque>

/*
	Examples on template template parameters
*/

template <
	template <typename> class OutContainer,
	template <typename> class InContainer,
	typename T>
OutContainer<T> resequence(const InContainer<T>& in)
{
	OutContainer<T> out;
	for (auto x : in)
		out.push_back(x);
	return out;
}

template <typename T, template <typename> class Container>
class Builder {
	Container<T> data_;
public:
	void add(const T& t) { data_.push_back(t); }
	void print() const {
		for (const auto& x : data_) std::cout << x << " ";
		std::cout << std::endl;
	}
};


