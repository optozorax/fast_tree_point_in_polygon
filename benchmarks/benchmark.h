#pragma once

#include <utility>
#include <ftpip/ftpip.h>

struct BenchmarkResult
{
	double time_treeCreate;
	double time_my;
	double time_standard;

	int wrongPixels;
	int treeHeight;
};

std::pair<double, double> calcBenchmark(const std::vector<spob::vec2>& polygon, int size, int& wrongCount, int& treeSize);