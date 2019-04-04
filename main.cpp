#include "visualizations/visualize.h"

int main() {
	using namespace ftpip;
	using namespace std;
	using namespace spob;

	vector<vec2> poly1 = {{0, 0}, {2, 1}, {0.5, 3}};
	vector<vec2> poly2 = {{0, 4}, {3, 5}, {5, 7}, {7, 4}, {6, 3}, {4, 0}, {3, 3}};
	vec2 middle = (poly1[0] + poly1[1])/2.0;
	TreeElem_ptr tree = std::make_shared<TreeElem>();

	//tree->type = TreeElem::TRUE;

	// check_work(poly2, tree, L"triangle", 500);
	// drawAllTree(L"triangle_tree", 100, 10, 10, tree, poly2);

	//-------------------------------------------------------------------------

	// tree->type = TreeElem::NEXT;
	// tree->check = std::make_shared<QuadCheck>(vec2(0, 0), vec2(1, 1));
	// //tree->check = std::make_shared<HalfQuadCheck>(vec2(0, 0), vec2(1, 1), vec2(1, 0));
	// //tree->check = std::make_shared<HalfQuadCheck>(vec2(0, 0), vec2(1, 1), vec2(0, 1));
	// //tree->check = std::make_shared<HalfQuadCheck>(vec2(1, 0), vec2(0, 1), vec2(0, 0));
	// //tree->check = std::make_shared<HalfQuadCheck>(vec2(1, 0), vec2(0, 1), vec2(1, 1));
	// tree->if_true = std::make_shared<TreeElem>();
	// tree->if_true->type = TreeElem::TRUE;
	// tree->if_false = std::make_shared<TreeElem>();
	// tree->if_false->type = TreeElem::FALSE;

	// check_work(poly1, tree, L"triangle", 500);
	// drawAllTree(L"triangle_tree", 100, 10, 10, tree, poly1);

	//-------------------------------------------------------------------------

	// tree->type = TreeElem::NEXT;
	// tree->check = std::make_shared<QuadCheck>(poly1[0], poly1[2]);

	// tree->if_true = std::make_shared<TreeElem>();
	// tree->if_true->type = TreeElem::NEXT;
	// tree->if_true->check = std::make_shared<HalfQuadCheck>(poly1[0], poly1[2], vec2(middle.x, poly1[0].y), poly1[0], poly1[2]);
	// tree->if_true->if_true = std::make_shared<TreeElem>();
	// tree->if_true->if_true->type = TreeElem::TRUE;
	// tree->if_true->if_false = std::make_shared<TreeElem>();
	// tree->if_true->if_false->type = TreeElem::FALSE;

	// tree->if_false = std::make_shared<TreeElem>();
	// tree->if_false->type = TreeElem::NEXT;
	// tree->if_false->check = std::make_shared<HalfQuadCheck>(poly1[1], poly1[2], vec2(middle.x, poly1[1].y), poly1[0], poly1[2]);
	// tree->if_false->if_true = std::make_shared<TreeElem>();
	// tree->if_false->if_true->type = TreeElem::TRUE;
	// tree->if_false->if_false = std::make_shared<TreeElem>();
	// tree->if_false->if_false->type = TreeElem::FALSE;

	// check_work(poly1, tree, L"triangle", 500);
	// drawAllTree(L"triangle_tree", 100, 10, 10, tree, poly1);

	//-------------------------------------------------------------------------

	tree->type = TreeElem::NEXT;
	tree->isTransform = true;
	space2 crd = makeLine2(poly1[0], poly1[1]);
	tree->transform = getToMatrix(crd);
	auto poly11 = toMas(crd, poly1);

	tree->check_1 = std::make_shared<QuadCheck>(poly11[0], poly11[2]);
	tree->check_2 = std::make_shared<QuadCheck>(getUpLeft(poly11[1], poly11[2]), getDownRight(poly11[1], poly11[2]));

	tree->if_1 = std::make_shared<TreeElem>();
	tree->if_1->type = TreeElem::FINAL;
	tree->if_1->check_final = std::make_shared<HalfQuadCheck>(poly11[0], poly11[2], vec2(poly11[2].x, poly11[0].y), poly11[0], poly11[2]);

	tree->if_2 = std::make_shared<TreeElem>();
	tree->if_2->type = TreeElem::FINAL;
	tree->if_2->check_final = std::make_shared<HalfQuadCheck>(poly11[1], poly11[2], vec2(poly11[2].x, poly11[1].y), getUpLeft(poly11[1], poly11[2]), getDownRight(poly11[1], poly11[2]));

	check_work(poly1, tree, L"triangle", 500);
	drawAllTree(L"triangle_tree", 100, 10, 10, tree, poly1);
}