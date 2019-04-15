#include <chrono>
#include <ftpip/find_borders.h>
#include <ftpip/standard.h>
#include <spob/spob2glm.h>
#include "benchmark.h"

using namespace spob;
using namespace ftpip;

//-----------------------------------------------------------------------------
BenchmarkResult calcBenchmark(const std::vector<vec2>& polygon, int size) {
	BenchmarkResult result;

	FindBorders brd(size, 0);
	brd.process(polygon);
	brd.finish();
	auto sz = brd.getCalculatedSize();

	auto start = std::chrono::high_resolution_clock::now();
	TreeElem_ptr tree = std::make_shared<TreeElem>();
	makeTree(tree, polygon, polygon, 100);
	auto end = std::chrono::high_resolution_clock::now();
	result.time_treeCreate = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	result.treeHeight = calcHeight(tree);

	//-------------------------------------------------------------------------
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < sz.x; ++i) {
		for (int j = 0; j < sz.y; ++j) {
			auto pos = glm::vec3(spob2glm(brd.to(vec2(i, j))), 1);
			bool isTreeInside = isInside(tree, pos);
		}
	}
	end = std::chrono::high_resolution_clock::now();
	result.time_my = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//-------------------------------------------------------------------------
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < sz.x; ++i) {
		for (int j = 0; j < sz.y; ++j) {
			auto pos = brd.to(vec2(i, j));
			bool isTrulyInside = isPointInsidePolygon(polygon, pos);
		}
	}
	end = std::chrono::high_resolution_clock::now();
	result.time_standard = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//-------------------------------------------------------------------------
	result.wrongPixels = 0;
	for (int i = 0; i < sz.x; ++i) {
		for (int j = 0; j < sz.y; ++j) {
			auto pos = brd.to(vec2(i, j));
			auto pos1 = glm::vec3(spob2glm(pos), 1);
			bool isTreeInside = isInside(tree, pos1);
			bool isTrulyInside = isPointInsidePolygon(polygon, pos);
			bool isOnPolyLine = pointOnPolyline(polygon, pos);
			if (isTreeInside != isTrulyInside && !isOnPolyLine)
				result.wrongPixels++;
		}
	}

	return result;
}