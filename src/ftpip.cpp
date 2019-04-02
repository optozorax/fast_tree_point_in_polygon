#include <cassert>
#include <ftpip/ftpip.h>

namespace ftpip
{

//-----------------------------------------------------------------------------
QuadCheck::QuadCheck(const spob::vec2& up_left, spob::vec2& down_right) : up_left(up_left), down_right(down_right) {
	x_check.push_back(Compare1(false, up_left.x));
	x_check.push_back(Compare1(true, down_right.x));

	y_check.push_back(Compare1(false, up_left.y));
	y_check.push_back(Compare1(true, down_right.y));
}

//-----------------------------------------------------------------------------
QuadCheck::QuadCheck(const spob::vec2& up_left, spob::vec2& down_right, const spob::vec2& last_up_left, spob::vec2& last_down_right) : up_left(up_left), down_right(down_right) {
	if (up_left.x != last_up_left.x) 
		x_check.push_back(Compare1(false, up_left.x));

	if (down_right.x != last_down_right.x) 
		x_check.push_back(Compare1(true, down_right.x));

	if (up_left.y != last_up_left.x) 
		y_check.push_back(Compare1(false, up_left.y));

	if (down_right.y != last_down_right.y) 
		y_check.push_back(Compare1(true, down_right.y));
}

//-----------------------------------------------------------------------------
bool QuadCheck::isInside(const spob::vec2& p) {
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
spob::vec2 getUpLeft(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) {
	return spob::vec2(std::min(a.x, b.x, c.x), std::min(a.y, b.y, c.y));
}

//-----------------------------------------------------------------------------
spob::vec2 getDownRight(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) {
	return spob::vec2(std::max(a.x, b.x, c.x), std::max(a.y, b.y, c.y));
}

//-----------------------------------------------------------------------------
HalfQuadCheck::HalfQuadCheck(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c) : QuadCheck(getUpLeft(a, b, c), getDownRight(a, b, c)) {
	assert((a.x == c.x && b.y == c.y) || (a.y == c.y && b.x == c.x));

}

//-----------------------------------------------------------------------------
HalfQuadCheck::HalfQuadCheck(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c,const spob::vec2& last_up_left, spob::vec2& last_down_right) : QuadCheck(getUpLeft(a, b, c), getDownRight(a, b, c), last_up_left, last_down_right) {
	if (a.x == c.x && b.y == c.y) {
		xy_check = Compare2();
		//%TODO
	} else if (a.y == c.y && b.x == c.x) {
		//%TODO
	} else {
		// Сюда передан не тот треугольник, который необходимо передать по контракту
		throw std::exception();
	}
}

//-----------------------------------------------------------------------------
bool HalfQuadCheck::isInside(const spob::vec2& p) {
	if (!QuadCheck::isInside(p))
		return false;

	if (!xy_check.check(p.x, p.y))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
double HalfQuadCheck::getComplexity(void) const {
	const double compare2_complexity = 2;
	return QuadCheck::getComplexity() + xy_check.size() * compare2_complexity;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double calcComplexity(TreeElem_ptr tree) {
	const double matrix_complexity = 16;
	if ()
}

//-----------------------------------------------------------------------------
void makeTree(TreeElem_ptr tree, const std::vector<vec2>& points) {

}

//-----------------------------------------------------------------------------
bool isInside(TreeElem_ptr tree, const spob::vec2& p) {
	switch (tree->type) {
		case TreeElem::TRUE: {
			return true;
		} break;
		case TreeElem::FALSE: {
			return false;
		} break;
		case TreeElem::NEXT: {
			vec2 newp = p;
			if (tree->isTransform)
				newp = glm2spob(tree->transform * spob2glm(p));
			
			if (tree->check(newp)) 
				return isInside(tree->if_true, p);
			else
				return isInside(tree->if_false, p);
		} break;
	}
}

};