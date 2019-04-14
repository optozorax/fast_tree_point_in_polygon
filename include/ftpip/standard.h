#pragma once

#include <vector>
#include <spob/spob.h>

namespace ftpip
{
	bool isPointInsidePolygon(const std::vector<spob::vec2>& mas,
	                          const spob::vec2& p);

	bool pointOnPolyline(const std::vector<spob::vec2>& poly, spob::vec2 p);

	std::vector<std::vector<spob::vec2> > intersectPolygons(
		const std::vector<spob::vec2>& polygon1, 
		const std::vector<spob::vec2>& polygon2
	);

	double calculateArea(const std::vector<spob::vec2>& poly);
};