#include <vector>
#include <deque>

#include "template_template_params.hpp"

int main() {
	std::vector<int> v = { 1, 2, 4, 5 };

	Builder<int, std::vector> b;

	b.add(1);
	b.print();

	auto d = resequence<std::deque>(v);
}
