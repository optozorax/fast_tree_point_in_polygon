#pragma once

#include <vector>
#include <spob/spob.h>

namespace ftpip
{
	bool isPointInsidePolygon(const std::vector<spob::vec2>& mas,
	                          const spob::vec2& p);
};