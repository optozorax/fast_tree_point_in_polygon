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

BenchmarkResult calcBenchmark(const std::vector<spob::vec2>& polygon, int size);