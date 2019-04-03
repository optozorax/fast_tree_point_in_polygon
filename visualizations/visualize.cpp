#include <ftpip/standard.h>
#include "visualize.h"
#include "find_borders.h"

#include <twg/image/image_drawing.h>
#include <twg/image.h>

void check_work(const std::vector<spob::vec2>& polygon, ftpip::TreeElem_ptr tree, const std::wstring& name, int size) {
	using namespace twg;
	using namespace spob;
	using namespace ftpip;

	spob::FindBorders brd(size-50, 50);
	brd.process(polygon);
	brd.finish();
	ImageDrawing_aa img(brd.getCalculatedSize());
	img.clear(White);

	for (int i = 0; i < img.width(); ++i) {
		for (int j = 0; j < img.height(); ++j) {
			bool isTrulyInside = isPointInsidePolygon(polygon, brd.to(vec2(i, j)));
			bool isTreeInside = isInside(tree, brd.to(vec2(i, j)));

			if (isTrulyInside == isTreeInside) {
				img[Point_i(i, j)] = getColorBetween(0.5, Green, White);
			} else {
				img[Point_i(i, j)] = getColorBetween(0.5, Red, White);
			}
		}
	}

	Polygon_d poly;
	for (auto& i : polygon)
		poly.array.push_back(brd.from(i));
	img.setPen(Pen(2, Black));
	img.drawPolyline(poly);

	saveToPng(&img, name + L".png");
}