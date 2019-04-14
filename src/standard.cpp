#include <clipper.hpp>
#include <ftpip/standard.h>
#include <ftpip/find_borders.h>

namespace ftpip
{

//-----------------------------------------------------------------------------
/** Big thanks to M. Galetzka and P. Glauner. This code was copied from https://github.com/pglauner/point_in_polygon . */

//-----------------------------------------------------------------------------
struct Line {
	spob::vec2 p1;
	spob::vec2 p2;
};

//-----------------------------------------------------------------------------
inline int ccw(const spob::vec2& p0, const spob::vec2& p1, const spob::vec2& p2) {
	double dx1 = p1.x - p0.x;
	double dy1 = p1.y - p0.y;
	double dx2 = p2.x - p0.x;
	double dy2 = p2.y - p0.y;
	double dx1dy2 = dx1 * dy2;
	double dx2dy1 = dx2 * dy1;

	if (dx1dy2 > dx2dy1) {
		return 1;
	} else if (dx1dy2 < dx2dy1) {
		return -1;
	}
	else {
		if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
			return -1;
		} else if (dx1 * dx1 + dy1 * dy1 >= dx2 * dx2 + dy2 * dy2) {
			return 0;
		} else {
			return 1;
		}
	}
}

//-----------------------------------------------------------------------------
inline int intersect(const Line& line1, const Line& line2) {
	int ccw11 = ccw(line1.p1, line1.p2, line2.p1); if (ccw11 == 0) return 1;
	int ccw12 = ccw(line1.p1, line1.p2, line2.p2); if (ccw12 == 0) return 1;
	if (!(ccw11 * ccw12 < 0))
		return 0;
	int ccw21 = ccw(line2.p1, line2.p2, line1.p1); if (ccw21 == 0) return 1;
	int ccw22 = ccw(line2.p1, line2.p2, line1.p2); if (ccw22 == 0) return 1;
	return (ccw21 * ccw22 < 0) ? 1 : 0;
}

//-----------------------------------------------------------------------------
inline int getNextIndex(int n, int current) {
	return current == n - 1 ? 0 : current + 1;
}

//-----------------------------------------------------------------------------

bool isPointInsidePolygon(const std::vector<spob::vec2>& polygon1, const spob::vec2& testPoint1) {
	auto testPoint = testPoint1;
	auto polygon = polygon1;
	int n = polygon.size();
	Line xAxis;
	Line xAxisPositive;

	spob::vec2 startPoint;
	spob::vec2 endPoint;
	Line edge;
	Line testPointLine;

	int i;
	double startNodePosition;
	int count;
	int seenPoints;

	// Initial start point
	startPoint.x = 0;
	startPoint.y = 0;

	// Create axes
	xAxis.p1.x = 0;
	xAxis.p1.y = 0;
	xAxis.p2.x = 0;
	xAxis.p2.y = 0;
	xAxisPositive.p1.x = 0;
	xAxisPositive.p1.y = 0;
	xAxisPositive.p2.x = 0;
	xAxisPositive.p2.y = 0;

	startNodePosition = -1;

	// Is testPoint on a node?
	// Move polygon to 0|0
	// Enlarge axes
	for (i = 0; i < n; i++) {
		if (testPoint.x == polygon[i].x && testPoint.y == polygon[i].y) {
			return 1;
		}

		// Move polygon to 0|0
		polygon[i].x -= testPoint.x;
		polygon[i].y -= testPoint.y;

		// Find start point which is not on the x axis
		if (polygon[i].y != 0) {
			startPoint.x = polygon[i].x;
			startPoint.y = polygon[i].y;
			startNodePosition = i;
		}

		// Enlarge axes
		if (polygon[i].x > xAxis.p2.x) {
			xAxis.p2.x = polygon[i].x;
			xAxisPositive.p2.x = polygon[i].x;
		}
		if (polygon[i].x < xAxis.p1.x) {
			xAxis.p1.x = polygon[i].x;
		}
	}

	// Move testPoint to 0|0
	testPoint.x = 0;
	testPoint.y = 0;
	testPointLine.p1 = testPoint;
	testPointLine.p2 = testPoint;

	// Is testPoint on an edge?
	for (i = 0; i < polygon.size(); i++) {
		edge.p1 = polygon[i];
		// Get correct index of successor edge
		edge.p2 = polygon[getNextIndex(polygon.size(), i)];
		if (intersect(testPointLine, edge) == 1) {
			return 1;
		}
	}

	// No start point found and point is not on an edge or node
	// --> point is outside
	if (startNodePosition == -1) {
		return 0;
	}

	count = 0;
	seenPoints = 0;
	i = startNodePosition;

	// Consider all edges
	while (seenPoints < n) {

		double savedX = polygon[getNextIndex(n, i)].x;
		int savedIndex = getNextIndex(n, i);

		// Move to next point which is not on the x-axis
		do {
			i = getNextIndex(n, i);
			seenPoints++;
		} while (polygon[i].y == 0);
		// Found end point
		endPoint.x = polygon[i].x;
		endPoint.y = polygon[i].y;

		// Only intersect lines that cross the x-axis
		if (startPoint.y * endPoint.y < 0) {
			edge.p1 = startPoint;
			edge.p2 = endPoint;

			// No nodes have been skipped and the successor node
			// has been chosen as the end point
			if (savedIndex == i) {
				count += intersect(edge, xAxisPositive);
			}
			// If at least one node on the right side has been skipped,
			// the original edge would have been intersected
			// --> intersect with full x-axis
			else if (savedX > 0) {
				count += intersect(edge, xAxis);
			}
		}
		// End point is the next start point
		startPoint = endPoint;
	}

	// Odd count --> in the polygon (1)
	// Even count --> outside (0)
	return count % 2;
}

using namespace spob;

//-----------------------------------------------------------------------------
double calculateArea(const std::vector<vec2>& poly) {
	if (poly.size() == 0)
		return 0;

	double sum = 0;
	for (int i = 0; i < poly.size()-1; ++i)
		sum += poly[i].x * poly[i+1].y - poly[i+1].x * poly[i].y;
	sum += poly[poly.size() - 1].x * poly[0].y - poly[0].x * poly[poly.size() - 1].y;

	sum *= 0.5;

	if (sum < 0) sum = -sum;

	return sum;
}

//-----------------------------------------------------------------------------
std::vector<std::vector<vec2> > intersectPolygons(const std::vector<vec2>& polygon1, const std::vector<vec2>& polygon2) {
	std::vector<std::vector<vec2> > result;

	if (polygon1.empty() || polygon2.empty())
		return result;

	FindBorders brd(10000000, 0);
	for (auto& i : polygon1)
		brd.process(i);
	for (auto& i : polygon2)
		brd.process(i);
	brd.finish();

	using namespace ClipperLib;
	Path subj, clip;
	Paths solution;

	for (const auto& i : polygon1) {
		auto p = brd.from(i);
		subj.push_back(IntPoint(p.x, p.y));
	}
	for (const auto& i : polygon2) {
		auto p = brd.from(i);
		clip.push_back(IntPoint(p.x, p.y));
	}

	Clipper c;
	c.AddPath(subj, ptSubject, true);
	c.AddPath(clip, ptClip, true);
	c.Execute(ctIntersection, solution, pftNonZero, pftNonZero);
	
	for (auto& i : solution) {
		result.push_back({});
		for (auto& j : i)
			result.back().push_back(brd.to(vec2(j.X, j.Y)));
	}

	return result;
}

//-----------------------------------------------------------------------------
template<class T, class Func>
void forEachLineInPolygonIndx(const std::vector<T>& mas, const Func& func) {
	if (mas.size() > 1) {
		for (int i = 0; i < mas.size() - 1; ++i)
			func(i, i + 1);
		func(mas.size() - 1, 0);
	}
}

//-----------------------------------------------------------------------------
template<class T, class Func>
void forEachLineInPolygonObj(const std::vector<T>& mas, const Func& func) {
	forEachLineInPolygonIndx(mas, [&] (int i, int j) {
		func(mas[i], mas[j]);
	});
}

//-----------------------------------------------------------------------------
bool pointOnPolyline(const std::vector<spob::vec2>& poly, spob::vec2 p) {
	bool returned = false;
	forEachLineInPolygonObj(poly, [&] (vec2 a, vec2 b) {
		double t = makeLine2(a, b).to(p);
		double h = distance(makeLine2(a, b), p);
		returned |= t >= 0 && t < 1 && h < 0.00001;
	});

	return returned;
}

};