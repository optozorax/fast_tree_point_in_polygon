#include <cassert>
#include <algorithm>
#include <ftpip/ftpip.h>

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
	assert((a.x == c.x && b.y == c.y) || (a.y == c.y && b.x == c.x));
	double k = (b.y-a.y)/(b.x-a.x);
	xy_check = Check::Compare2(true, k, a.y-a.x*k);
	if (!xy_check.check(c.x, c.y)) xy_check.less = !xy_check.less;
}

//-----------------------------------------------------------------------------
HalfQuadCheck::HalfQuadCheck(const spob::vec2& a, const spob::vec2& b, const spob::vec2& c,const spob::vec2& last_up_left, const spob::vec2& last_down_right) : QuadCheck(getUpLeft(a, b, c), getDownRight(a, b, c), last_up_left, last_down_right) {
	assert((a.x == c.x && b.y == c.y) || (a.y == c.y && b.x == c.x));
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
void makeTree(TreeElem_ptr tree, const std::vector<spob::vec2>& points, int depth) {

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