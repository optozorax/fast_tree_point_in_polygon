#include <ftpip/standard.h>
#include <functional>
#include "visualize.h"
#include "find_borders.h"

#include <twg/image/image_drawing.h>
#include <twg/image.h>

#undef TRUE
#undef FALSE

using namespace twg;
using namespace spob;
using namespace ftpip;

//-----------------------------------------------------------------------------
void check_work(const std::vector<spob::vec2>& polygon, ftpip::TreeElem_ptr tree, const std::wstring& name, int size) {
	FindBorders brd(size-50, 50);
	brd.process(polygon);
	brd.finish();
	ImageDrawing_aa img(brd.getCalculatedSize());
	img.clear(White);

	for (int i = 0; i < img.width(); ++i) {
		for (int j = 0; j < img.height(); ++j) {
			bool isTrulyInside = isPointInsidePolygon(polygon, brd.to(vec2(i, j)));
			bool isTreeInside = isInside(tree, glm::vec3(spob2glm(brd.to(vec2(i, j))), 1));

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

//-----------------------------------------------------------------------------
void drawCurrentTree(ImageDrawing_aa& img, const FindBorders& brd, TreeElem_ptr tree, TreeElem_ptr current, const std::vector<spob::vec2>& polygon, const glm::mat3& currentTransformation) {
	/* 
	Должно быть 4 цвета:
		Зеленый: попадая в эту область, точка считается принадлежащей полигону
		Красный: попадая в эту область, точка считается не принадлежащей полигону
		Синий: в эту область точка попадает когда check возвращает true
		Оранжевый: в эту область точка попадает когда check возвращает false
		Белый: данная точка не может попасть в этот узел
			Тогда надо передавать изначальное дерево и необходимый для рисования узел, чтобы можно было определить такие точки
	Что здесь должно рисоваться?
		Если нет преобразования

		Если есть

	Алгоритм рисования: 
		Перебираем все точки и определяем те, 
	*/

	auto transform = currentTransformation;
	if (current->isTransform)
		transform = current->transform * transform;

	img.resize(brd.getCalculatedSize());
	img.clear(White);
	for (int i = 0; i < img.width(); ++i) {
		for (int j = 0; j < img.height(); ++j) {
			vec2 pos(i, j);
			glm::vec3 gpos1(spob2glm(brd.to(pos)), 1);
			glm::vec3 gpos = transform * gpos1;
			Color clr = White;
			if (isReachable(tree, current, gpos1)) {
				if (current->type == TreeElem::TRUE)
					clr = Green;
				else if (current->type == TreeElem::FALSE)
					clr = Red;
				else if (current->type == TreeElem::FINAL) {
					if (current->check_final->isInside(gpos))
						clr = Green;
					else
						clr = Red;
				} else {
					if (current->check_1->isInside(gpos)) {
						if (current->if_1->type == TreeElem::TRUE)
							clr = Green;
						else if (current->if_1->type == TreeElem::FALSE)
							clr = Red;
						else
							clr = Blue;
					} else if (current->check_2->isInside(gpos)) {
						if (current->if_2->type == TreeElem::TRUE)
							clr = Green;
						else if (current->if_2->type == TreeElem::FALSE)
							clr = Red;
						else
							clr = Miku;
					} else 
						clr = Red;
				}
			}
			img[pos] = getColorBetween(0.5, clr, White);
		}
	}

	Polygon_d poly;
	for (auto& i : polygon)
		poly.array.push_back(brd.from(i));
	img.setPen(Pen(2, Black));
	img.drawPolyline(poly);
}

//-----------------------------------------------------------------------------
void drawAllTree(const std::wstring& filename, 
				 int size, int insideBorder, int padding, 
                 TreeElem_ptr tree, 
                 const std::vector<spob::vec2>& polygon) {
	spob::FindBorders brd(size-2*insideBorder, insideBorder);
	brd.process(polygon);
	brd.finish();

	auto sz = brd.getCalculatedSize();
	int height = calcHeight(tree);
	vec2 newSize(pow(2.0, height-1) * (sz.x + padding) + padding, height * (sz.y + padding) + padding);

	ImageDrawing_aa main_img(newSize);
	main_img.clear(White);

	std::function<void(TreeElem_ptr, vec2, vec2, vec2, glm::mat3)> drawAllTreeRecur;
	drawAllTreeRecur = [&main_img, &sz, &padding, &polygon, &brd, &drawAllTreeRecur, &tree] (TreeElem_ptr _tree, vec2 a, vec2 b, vec2 last, glm::mat3 current) {
		ImageDrawing_aa img(sz);
		drawCurrentTree(img, brd, tree, _tree, polygon, current);

		if (_tree->isTransform)
			current = _tree->transform * current;

		vec2 pos = (a + b) / 2.0;
		pos.y *= sz.y + padding;
		
		vec2 dstStart = vec2(0, padding) + pos - vec2(sz.x, 0)/2.0;

		img.drawTo(&main_img, dstStart, Point_i(0, 0), sz, sz);

		Polygon_d poly;
		poly.array.push_back(dstStart);
		poly.array.push_back(dstStart + vec2(sz.x, 0));
		poly.array.push_back(dstStart + sz);
		poly.array.push_back(dstStart + vec2(0, sz.y));
		main_img.setPen(Pen(2, Black));
		main_img.drawPolyline(poly);

		if (_tree != tree) {
			main_img.drawLine(last, dstStart + vec2(sz.x/2.0, 0));
		}

		if (_tree->type == TreeElem::NEXT) {
			drawAllTreeRecur(_tree->if_1, a + vec2(0, 1), (a + b)/2.0 + vec2(0, 1), dstStart + vec2(0, sz.y), current);
			drawAllTreeRecur(_tree->if_2, (a + b)/2.0 + vec2(0, 1), b + vec2(0, 1), dstStart + sz, current);
		}
	};

	drawAllTreeRecur(tree, vec2(0, 0), vec2(newSize.x, 0), vec2(0, 0), glm::mat3(1.0));

	saveToPng(&main_img, filename + L".png");
}