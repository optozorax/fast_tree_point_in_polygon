#include <iostream>
#include "benchmark.h"

void print_benchmark(const std::vector<spob::vec2>& mas) {
	int wrong = 0, treeSize = 0;
	auto res = calcBenchmark(mas, 2000, wrong, treeSize);
	std::cout << "Speed ratio: " << res.second/res.first << ", 1-time: " << res.first << "ms, 2-time: " << res.second << "ms, wrong pixels: " << wrong << ", tree height: " << treeSize << std::endl;
}

int main() {
	using namespace std;
	using namespace spob;
	using namespace ftpip;

	vector<vec2> poly2;
	for (int i = 0; i < 500; i++) {
		poly2.push_back(polar2cartesian(vec2(2.0 * i/500.0*_SPOB_PI, 1)));
		poly2.back().x *= 0.3;
	}

	print_benchmark({{0, 4}, {3, 5}, {5, 7}, {7, 4}, {6, 3}, {4, 0}, {3, 3}});
	print_benchmark(poly2);
	print_benchmark({{0, 0}, {0, 1}, {1, 1}, {1, 0}});
	print_benchmark({{0, 0}, {5, 1}, {4, 6}, {-1, 5}});
	print_benchmark({{0, 0}, {0, 1}, {2, 1}, {2, 0}});
	print_benchmark({{0, 0}, {5, 1}, {3, 11}, {-2, 10}});
	print_benchmark({{0, 0}, {0, 1}, {2, 0}});
	print_benchmark({{0, 0}, {2, 1}, {1, 3}});
	print_benchmark({{0, 1}, {2, 0}, {0, 0}});
	print_benchmark({{2, 1}, {1, 3}, {0, 0}});
	print_benchmark({{2, 0}, {0, 0}, {0, 1}});
	print_benchmark({{1, 3}, {0, 0}, {2, 1}});

	system("pause");
}