#include <cassert>
#include <algorithm>
#include <ftpip/ftpip.h>
#include <ftpip/standard.h>
#include <spob/spob2glm.h>

namespace ftpip
{

//-----------------------------------------------------------------------------
spob::vec2 getUpLeft(const spob::vec2& a, const spob::vec2& b) {
	return spob::vec2(std::min(a.x, b.x), std::min(a.y, b.y));
}

//-----------------------------------------------------------------------------
spob::vec2 getDownRight(const spob::vec2& a, const spob::vec2& b) {
	return spob::vec2(std::max(a.x, b.x), std::max(a.y, b.y));
}

//-----------------------------------------------------------------------------
spob::vec2 getUpLeft(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) {
	return spob::vec2(std::min(a.x, std::min(b.x, c.x)), std::min(a.y, std::min(b.y, c.y)));
}

//-----------------------------------------------------------------------------
spob::vec2 getDownRight(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) {
	return spob::vec2(std::max(a.x, std::max(b.x, c.x)), std::max(a.y, std::max(b.y, c.y)));
}

//-----------------------------------------------------------------------------
spob::vec2 getUpLeft(const std::vector<spob::vec2>& mas) {
	auto result = mas[0];
	for (auto& i : mas)
		result = getUpLeft(result, i);
	return result;
}

//-----------------------------------------------------------------------------
spob::vec2 getDownRight(const std::vector<spob::vec2>& mas) {
	auto result = mas[0];
	for (auto& i : mas)
		result = getDownRight(result, i);
	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QuadCheck::QuadCheck(const spob::vec2& up_left, const spob::vec2& down_right) : up_left(up_left), down_right(down_right) {
	x_check.push_back(Check::Compare1(false, up_left.x));
	x_check.push_back(Check::Compare1(true, down_right.x));

	y_check.push_back(Check::Compare1(false, up_left.y));
	y_check.push_back(Check::Compare1(true, down_right.y));
}

//-----------------------------------------------------------------------------
QuadCheck::QuadCheck(const spob::vec2& up_left, const spob::vec2& down_right, const spob::vec2& last_up_left, const spob::vec2& last_down_right) : up_left(up_left), down_right(down_right) {
	if (up_left.x != last_up_left.x) 
		x_check.push_back(Check::Compare1(false, up_left.x));

	if (down_right.x != last_down_right.x) 
		x_check.push_back(Check::Compare1(true, down_right.x));

	if (up_left.y != last_up_left.x) 
		y_check.push_back(Check::Compare1(false, up_left.y));

	if (down_right.y != last_down_right.y) 
		y_check.push_back(Check::Compare1(true, down_right.y));
}

//-----------------------------------------------------------------------------
bool QuadCheck::isInside(const glm::vec3& p) {
	for (auto& i : x_check) {
		if (!i.check(p.x))
			return false;
	}

	for (auto& i : y_check) {
		if (!i.check(p.y))
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
double QuadCheck::getComplexity(void) const {
	return x_check.size() + y_check.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
HalfQuadCheck::HalfQuadCheck(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) : QuadCheck(getUpLeft(a, b, c), getDownRight(a, b, c)) {
	//assert((a.x == c.x && b.y == c.y) || (a.y == c.y && b.x == c.x));
	double k = (b.y-a.y)/(b.x-a.x);
	xy_check = Check::Compare2(true, k, a.y-a.x*k);
	if (!xy_check.check(c.x, c.y)) xy_check.less = !xy_check.less;
}

//-----------------------------------------------------------------------------
HalfQuadCheck::HalfQuadCheck(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c,const spob::vec2& last_up_left, const spob::vec2& last_down_right) : QuadCheck(getUpLeft(a, b, c), getDownRight(a, b, c), last_up_left, last_down_right) {
	//assert((a.x == c.x && b.y == c.y) || (a.y == c.y && b.x == c.x));
	double k = (b.y-a.y)/(b.x-a.x);
	xy_check = Check::Compare2(true, k, a.y-a.x*k);
	if (!xy_check.check(c.x, c.y)) xy_check.less = !xy_check.less;
}

//-----------------------------------------------------------------------------
bool HalfQuadCheck::isInside(const glm::vec3& p) {
	if (!QuadCheck::isInside(p))
		return false;

	if (!xy_check.check(p.x, p.y))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
double HalfQuadCheck::getComplexity(void) const {
	const double compare2_complexity = 2;
	return QuadCheck::getComplexity() + compare2_complexity;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double calcComplexity(TreeElem_ptr tree) {
	const double matrix_complexity = 16;
	double result = 0;
	if (tree->isTransform) 
		result += matrix_complexity;
	//result += tree->check->getComplexity();
	//result += (calcComplexity(tree->if_true) + calcComplexity(tree->if_false))/2.0;
	return result;
}

//-----------------------------------------------------------------------------
void makeTree(TreeElem_ptr tree, const std::vector<spob::vec2>& original_points, const std::vector<spob::vec2>& points, int depth) {
	using namespace std;
	using namespace spob;

	if (depth <= 0) {
		tree->type = TreeElem::TRUE;
		return;
	}

	auto make_rectangle = [] (vec2 a, vec2 b) -> vector<vec2> {
		vec2 ul = getUpLeft(a, b);
		vec2 dr = getDownRight(a, b);
		vector<vec2> result;
		result.push_back(ul);
		result.push_back(vec2(dr.x, ul.y));
		result.push_back(dr);
		result.push_back(vec2(ul.x, dr.y));
		return result;
	};

	auto rec = make_rectangle(getUpLeft(points), getDownRight(points));
	rec.push_back((rec[0] + rec[2])/2.0);
	bool isFullyInside = true;
	for (auto& i : rec) 
		isFullyInside &= isPointInsidePolygon(original_points, i) || pointOnPolyline(original_points, i);
	if (isFullyInside) {
		tree->type = TreeElem::TRUE;
		return;
	}

	auto calc_angles = [&] () -> vector<double> {
		vector<double> result;
		auto calc_angle = [&] (const vec2& a, const vec2& b, const vec2& c) {
			result.push_back(getRightAngle(a, b, c));
		};
		calc_angle(points.back(), points[0], points[1]);
		for (int i = 1; i < points.size()-1; i++)
			calc_angle(points[i-1], points[i], points[i+1]);
		calc_angle(points[points.size()-2], points.back(), points[0]);
		return result;
	};

	auto calc_edges = [&] () -> vector<double> {
		vector<double> result;
		auto calc_edge = [&] (const vec2& a, const vec2& b) {
			result.push_back((a-b).length());
		};
		for (int i = 0; i < points.size()-1; i++)
			calc_edge(points[i], points[i+1]);
		calc_edge(points.back(), points.front());
		return result;
	};

	auto check_rectangle = [&] () -> bool {
		// Проверяет что текущий полигон является прямоугольником
		if (points.size() != 4) return false;

		auto angles = calc_angles();
		
		auto min_angles = min_element(angles.begin(), angles.end());
		auto max_angles = max_element(angles.begin(), angles.end());

		return fabs(*min_angles-*max_angles) < 0.00001;
	};

	auto check_right_triangle = [&] () -> int {
		// Проверяет, что текущий полигон является прямоугольным треугольником
		if (points.size() != 3) return false;

		auto edges = calc_edges();

		for (int i = 0; i < edges.size(); i++) {
			double c2 = edges[i]; c2 *= c2;
			double ab2sum = 0;
			for (int j = 0; j < edges.size(); j++) {
				if (j != i)
					ab2sum += edges[j] * edges[j];
			}

			if (fabs(c2-ab2sum) < 0.00001)
				return i+1;
		}

		return false;
	};

	auto check_right_angle_line = [&] (const vec2& a, const vec2& b) -> bool {
		return fabs(a.x-b.x) < 0.00001 || fabs(a.y-b.y) < 0.00001;
	};

	auto is_have_right_angle_lines = [&] () -> bool {
		for (int i = 0; i < points.size()-1; i++)
			if (check_right_angle_line(points[i], points[i+1])) return true;
		if (check_right_angle_line(points.back(), points.front())) return true;
		return false;
	};

	int i = 0;
	if (i = check_right_triangle()) {
		i--;
		int a = (points.size() + (i+1)) % points.size();
		int b = i;
		int c = (points.size() + (i-1)) % points.size();
		if (is_have_right_angle_lines()) {
			tree->isTransform = false;
			tree->type = TreeElem::FINAL;
			tree->check_final = make_shared<HalfQuadCheck>(points[a], points[b], points[c]);
			vec2 middle = (points[a] + points[b] + points[c])/3.0;
			glm::vec3 d(spob2glm(middle), 1);
			#define check_new(A, B, C) if (!tree->check_final->isInside(d)) \
				tree->check_final = make_shared<HalfQuadCheck>(points[A], points[B], points[C]);
			check_new(a, c, b);
			check_new(c, b, a);
			check_new(c, a, b);
			check_new(b, a, c);
			check_new(b, c, a);
			#undef check_new

			if (!tree->check_final->isInside(d))
				throw std::exception();
		} else {
			tree->isTransform = true;
			space2 space = makeLine2(points[b], points[c]);
			tree->transform = getToMatrix(space);
			tree->type = TreeElem::FINAL;
			tree->check_final = make_shared<HalfQuadCheck>(space.to(points[a]), space.to(points[b]), space.to(points[c]));
			vec2 middle = (space.to(points[a]) + space.to(points[b]) + space.to(points[c]))/3.0;
			glm::vec3 d(spob2glm(middle), 1);
			#define check_new(A, B, C) if (!tree->check_final->isInside(d)) \
				tree->check_final = make_shared<HalfQuadCheck>(space.to(points[A]), space.to(points[B]), space.to(points[C]));
			check_new(a, c, b);
			check_new(c, b, a);
			check_new(c, a, b);
			check_new(b, a, c);
			check_new(b, c, a);
			#undef check_new

			if (!tree->check_final->isInside(d))
				throw std::exception();
		}
	} else if (check_rectangle()) {
		if (is_have_right_angle_lines()) {
			tree->isTransform = false;
			tree->type = TreeElem::FINAL;
			auto ul = getUpLeft(points), dr = getDownRight(points);
			tree->check_final = make_shared<QuadCheck>(ul, dr);
		} else {
			tree->isTransform = true;
			space2 space = makeLine2(points[0], points[1]);
			tree->transform = getToMatrix(space);
			tree->type = TreeElem::FINAL;
			auto newPoints = toMas(space, points);
			auto ul = getUpLeft(newPoints), dr = getDownRight(newPoints);
			tree->check_final = make_shared<QuadCheck>(ul, dr);
		}
	} else {
		/*
			Перебираем все стороны
				Если текущая сторона параллельна осям координат, то не делаем этого
					Делаем на основе текущей стороны систему координат
					Преобразуем текущий полигон к этой системе координат
				Рассчитываем ограничивающий прямоугольник текущего полигона
				Перебираем все точки полигона
					Создаем ограничивающий прямоуголньик слева и справа
					Находим пересечение текущего полигона и этих обоих прямоугольников
					Закидываем в массив структуру количества точек в каждом полигоне, а так же площадь каждого полигона, номер стороны, номер точки

					Делаем это как для X так и для Y

			Сортируем этот массив по какой-то метрике
			Выбираем наилучшую точку, и создаем два полигона, разделяющие её
			Расчитываем ограничивающие прямоугольники для каждого
			Задаем чеки для каждого полигона
			Вызываем рекурсивно эту функцию для остальных
		*/
		struct BestPoint
		{
			int a, b; // Номера текущей стороны
			bool isTransform;
			space2 space; // Преобразование системы координат для текущей стороны
			vector<vec2> poly1, poly2;
			double area1, area2;

			double getAreaRatio() const {
				return fabs(area1-area2); // Так работает очень хорошо
				//return fabs(poly1.size() - poly2.size())-(!isTransform); // Так не очень хорошо. Видимо надо для разных примеров разные метрики выбирать
			}

			bool operator<(const BestPoint& p) {
				return getAreaRatio() < p.getAreaRatio();
			}
		};

		BestPoint best;
		best.a = -1;

		auto process_point = [&best, &make_rectangle] (const vector<vec2>& points, int a, int b, int p, vec2 ul, vec2 dr, bool isTransform, space2 space) {
			auto r1 = make_rectangle(ul, vec2(points[p].x, dr.y));
			auto r2 = make_rectangle(vec2(points[p].x, ul.y), dr);

			auto r1_polys = intersectPolygons(points, r1);
			auto r2_polys = intersectPolygons(points, r2);

			// Пока я не знаю как обрабатывать такие ситуации
			// %TODO
			if (!(r1_polys.size() == 1 && r2_polys.size() == 1)) return;

			auto r1_poly = r1_polys[0];
			auto r2_poly = r2_polys[0];

			BestPoint current;
			current.a = a;
			current.b = b;
			current.space = space;
			current.isTransform = isTransform;
			current.poly1 = r1_poly;
			current.poly2 = r2_poly;
			current.area1 = calculateArea(r1_poly);
			current.area2 = calculateArea(r2_poly);

			if (best.a == -1 || current < best) 
				best = current;
		};

		auto process_edge = [&points, &check_right_angle_line, &process_point] (int a, int b) {
			space2 space = makeLine2(points[a], points[b]);
			vector<vec2> points_copy;
			bool isTransform = false;
			/*if (check_right_angle_line(points[a], points[b])) {
				isTransform = false;
				points_copy = points;
			} else*/ {
				isTransform = true;
				points_copy = toMas(space, points);
			}

			vec2 ul = getUpLeft(points_copy);
			vec2 dr = getDownRight(points_copy);

			for (int i = 0; i < points_copy.size(); ++i)
				process_point(points_copy, a, b, i, ul, dr, isTransform, space);
		};

		for (int i = 0; i < points.size() - 1; ++i)
			process_edge(i, i+1);
		process_edge(points.size() - 1, 0);

		// Получается что в любом случае получилось так, что полигон разделился на более чем один полигон.
		if (best.a == -1) { 
			//tree->type = TreeElem::TRUE;
			//return;
			throw std::exception();
		}

		auto orig_point_copy = original_points;

		tree->isTransform = best.isTransform;
		if (best.isTransform) {
			tree->transform = getToMatrix(best.space);
			orig_point_copy = toMas(best.space, original_points);
		}

		tree->type = TreeElem::NEXT;

		vec2 ul1 = getUpLeft(best.poly1);
		vec2 dr1 = getDownRight(best.poly1);
		tree->check_1 = std::make_shared<QuadCheck>(ul1, dr1);
		tree->if_1 = std::make_shared<TreeElem>();
		makeTree(tree->if_1, orig_point_copy , best.poly1, depth-1);

		vec2 ul2 = getUpLeft(best.poly2);
		vec2 dr2 = getDownRight(best.poly2);
		tree->check_2 = std::make_shared<QuadCheck>(ul2, dr2);
		tree->if_2 = std::make_shared<TreeElem>();
		makeTree(tree->if_2, orig_point_copy , best.poly2, depth-1);
	}
}

//-----------------------------------------------------------------------------
bool isInside(TreeElem_ptr tree, const glm::vec3& p) {
	auto newp = p;
	if (tree->isTransform)
		newp = tree->transform * newp;

	switch (tree->type) {
		case TreeElem::FINAL: {
			return tree->check_final->isInside(newp);
		} break;
		case TreeElem::TRUE: {
			return true;
		} break;
		case TreeElem::FALSE: {
			return false;
		} break;
		case TreeElem::NEXT: {
			if (tree->check_1->isInside(newp)) 
				return isInside(tree->if_1, newp);
			else if (tree->check_2->isInside(newp))
				return isInside(tree->if_2, newp);
			else
				return false;
		} break;
	}
}

//-----------------------------------------------------------------------------
bool isReachable(TreeElem_ptr tree, TreeElem_ptr current, const glm::vec3& p) {
	if (tree == current)
		return true;

	auto newp = p;
	if (tree->isTransform)
		newp = tree->transform * newp;

	switch (tree->type) {
		case TreeElem::FINAL: {
			return false;
		} break;
		case TreeElem::TRUE: {
			return false;
		} break;
		case TreeElem::FALSE: {
			return false;
		} break;
		case TreeElem::NEXT: {
			if (tree->check_1->isInside(newp)) 
				return isReachable(tree->if_1, current, newp);
			else if (tree->check_2->isInside(newp))
				return isReachable(tree->if_2, current, newp);
			else
				return false;
		} break;
	}
}

//-----------------------------------------------------------------------------
int calcHeight(TreeElem_ptr tree) {
	if (tree->type == TreeElem::NEXT)
		return std::max(calcHeight(tree->if_1), calcHeight(tree->if_2)) + 1;
	else 
		return 1;
}

};