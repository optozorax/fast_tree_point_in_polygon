#include <iostream>
#include "benchmark.h"

void print_benchmark(const std::vector<spob::vec2>& mas) {
	auto res = calcBenchmark(mas, 2000);
	std::cout 
		<< "N: " << mas.size() 
		<< ", speed ratio: " << res.time_standard/res.time_my 
		<< ", create tree time: " << res.time_treeCreate << "ms"
		<< ", my time: " << res.time_my << "ms"
		<< ", standard time: " << res.time_standard << "ms"
		<< ", wrong pixels: " << res.wrongPixels 
		<< ", tree height: " << res.treeHeight << std::endl;
}

int main() {
	using namespace std;
	using namespace spob;
	using namespace ftpip;

	vector<vec2> poly2;
	for (int i = 0; i < 50; i++) {
		poly2.push_back(polar2cartesian(vec2(2.0 * i/50.0*_SPOB_PI, 1)));
		poly2.back().x *= 0.3;
	}

	print_benchmark({{0, 0}, {0, 1}, {1, 1}, {1, 0}});
	print_benchmark({{0, 0}, {5, 1}, {4, 6}, {-1, 5}});
	print_benchmark({{0, 0}, {0, 1}, {2, 1}, {2, 0}});
	print_benchmark({{0, 0}, {5, 1}, {3, 11}, {-2, 10}});
	print_benchmark({{0, 0}, {0, 1}, {2, 0}});
	print_benchmark({{0, 0}, {2, 1}, {1, 3}});
	print_benchmark({{0, 4}, {3, 5}, {5, 7}, {7, 4}, {6, 3}, {4, 0}, {3, 3}});
	print_benchmark({{0, 1}, {0, 2}, {2, 2}, {2, 3}, {4, 1.5}, {2, 0}, {2, 1}});
	print_benchmark({{0, 1}, {0, 2}, {2, 2}, {2, 3}, {2, 4}, {0, 4}, {0, 5}, {2, 5}, {2, 6}, {4, 3}, {2, 0}, {2, 1}});
	print_benchmark({{0, 0}, {0, 2}, {1, 2}, {2, 4}, {2, 2}, {3, 1}, {5, 3}, {5, 2}, {6, 2}, {7, 1}, {8, 2}, {9, 1}, {9, 0}});
	print_benchmark(poly2);

	system("pause");
}