#include <iostream>
#include "visualizations/visualize.h"

#undef TRUE
#undef FALSE

void prove_work(const std::vector<spob::vec2>& mas, std::wstring mas_name) {
	using namespace ftpip;
	using namespace std;
	using namespace spob;

	TreeElem_ptr tree = std::make_shared<TreeElem>();
	makeTree(tree, mas, mas, 100);
	check_work(mas, tree, mas_name, 200, 7);
	std::cout << calcHeight(tree) << std::endl;
	//drawAllTree(mas_name + L"_tree", 50, 5, 5, tree, mas);
}

int main() {
	using namespace ftpip;
	using namespace std;
	using namespace spob;

	vector<vec2> circle;
	for (int i = 0; i < 50; i++) {
		circle.push_back(polar2cartesian(vec2(2.0 * i/50.0*_SPOB_PI, 1)));
		circle.back().x *= 0.3;
	}

	prove_work({{0, 0}, {0, 1}, {1, 1}, {1, 0}}, L"square");
	prove_work({{0, 0}, {5, 1}, {4, 6}, {-1, 5}}, L"slanted_square");

	prove_work({{0, 0}, {0, 1}, {2, 1}, {2, 0}}, L"rectangle");
	prove_work({{0, 0}, {5, 1}, {3, 11}, {-2, 10}}, L"slanted_rectangle");

	prove_work({{0, 0}, {0, 1}, {2, 0}}, L"right_triangle");
	prove_work({{0, 0}, {2, 1}, {1, 3}}, L"slanted_right_triangle");

	prove_work({{0, 4}, {3, 5}, {5, 7}, {7, 4}, {6, 3}, {4, 0}, {3, 3}}, L"poly2");
	prove_work({{0, 1}, {0, 2}, {2, 2}, {2, 3}, {4, 1.5}, {2, 0}, {2, 1}}, L"poly3");
	prove_work({{0, 1}, {0, 2}, {2, 2}, {2, 3}, {2, 4}, {0, 4}, {0, 5}, {2, 5}, {2, 6}, {4, 3}, {2, 0}, {2, 1}}, L"poly4");
	prove_work({{0, 0}, {0, 2}, {1, 2}, {2, 4}, {2, 2}, {3, 1}, {5, 3}, {5, 2}, {6, 2}, {7, 1}, {8, 2}, {9, 1}, {9, 0}}, L"poly5");

	prove_work(circle, L"CIRCLE");

	vector<vec2> mas = {{0, 4}, {3, 5}, {5, 7}, {7, 4}, {6, 3}, {4, 0}, {3, 3}};
	TreeElem_ptr tree = std::make_shared<TreeElem>();
	makeTree(tree, mas, mas, 100);
	drawAllTree(L"mas_tree", 100, 5, 5, tree, mas);

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

	// tree->type = TreeElem::NEXT;
	// tree->isTransform = true;
	// space2 crd = makeLine2(poly1[0], poly1[1]);
	// tree->transform = getToMatrix(crd);
	// auto poly11 = toMas(crd, poly1);

	// tree->check_1 = std::make_shared<QuadCheck>(poly11[0], poly11[2]);
	// tree->check_2 = std::make_shared<QuadCheck>(getUpLeft(poly11[1], poly11[2]), getDownRight(poly11[1], poly11[2]));

	// tree->if_1 = std::make_shared<TreeElem>();
	// tree->if_1->type = TreeElem::FINAL;
	// tree->if_1->check_final = std::make_shared<HalfQuadCheck>(poly11[0], poly11[2], vec2(poly11[2].x, poly11[0].y), poly11[0], poly11[2]);

	// tree->if_2 = std::make_shared<TreeElem>();
	// tree->if_2->type = TreeElem::FINAL;
	// tree->if_2->check_final = std::make_shared<HalfQuadCheck>(poly11[1], poly11[2], vec2(poly11[2].x, poly11[1].y), getUpLeft(poly11[1], poly11[2]), getDownRight(poly11[1], poly11[2]));

	// check_work(poly1, tree, L"triangle", 500, 50);
	// drawAllTree(L"triangle_tree", 200, 10, 10, tree, poly1);

	//-------------------------------------------------------------------------

	// tree->type = TreeElem::NEXT;
	// tree->isTransform = true;
	// space2 crd = makeLine2(poly2[2], poly2[3]);
	// tree->transform = getToMatrix(crd);
	// auto poly21 = toMas(crd, poly2);

	// middle = vec2(poly21.back().x, 0);

	// std::vector<vec2> mas1({poly21[0], poly21[1], poly21[2], middle, poly21.back()});
	// auto ul1 = getUpLeft(mas1), dr1 = getDownRight(mas1);
	// std::vector<vec2> mas2({middle, poly21[3], poly21[4], poly21[5], poly21[6]});
	// auto ul2 = getUpLeft(mas2), dr2 = getDownRight(mas2);
	// tree->check_1 = std::make_shared<QuadCheck>(ul1, dr1);
	// tree->check_2 = std::make_shared<QuadCheck>(ul2, dr2);

	// tree->if_1 = std::make_shared<TreeElem>();
	// tree->if_1->type = TreeElem::TRUE;

	// tree->if_2 = std::make_shared<TreeElem>();
	// tree->if_2->type = TreeElem::TRUE;

	// check_work(poly2, tree, L"star", 100, 10);
	// drawAllTree(L"star_tree", 100, 10, 10, tree, poly2);

	system("pause");
}