#include "visualizations/visualize.h"

int main() {
	using namespace ftpip;
	using namespace std;
	using namespace spob;

	vector<vec2> poly1 = {{0, 0}, {2, 1}, {1, 3}};
	vec2 middle = (poly1[0] + poly1[1])/2.0;
	TreeElem_ptr tree = std::make_shared<TreeElem>();

	//tree->type = TreeElem::TRUE;

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

	tree->type = TreeElem::NEXT;
	tree->check = std::make_shared<QuadCheck>(poly1[0], poly1[2]);

	tree->if_true = std::make_shared<TreeElem>();
	tree->if_true->type = TreeElem::NEXT;
	tree->if_true->check = std::make_shared<HalfQuadCheck>(poly1[0], poly1[2], vec2(middle.x, poly1[0].y), poly1[0], poly1[2]);
	tree->if_true->if_true = std::make_shared<TreeElem>();
	tree->if_true->if_true->type = TreeElem::TRUE;
	tree->if_true->if_false = std::make_shared<TreeElem>();
	tree->if_true->if_false->type = TreeElem::FALSE;

	tree->if_false = std::make_shared<TreeElem>();
	tree->if_false->type = TreeElem::NEXT;
	tree->if_false->check = std::make_shared<HalfQuadCheck>(poly1[1], poly1[2], vec2(middle.x, poly1[1].y), poly1[0], poly1[2]);
	tree->if_false->if_true = std::make_shared<TreeElem>();
	tree->if_false->if_true->type = TreeElem::TRUE;
	tree->if_false->if_false = std::make_shared<TreeElem>();
	tree->if_false->if_false->type = TreeElem::FALSE;

	makeTree(tree, poly1);
	check_work(poly1, tree, L"triangle", 500);
}