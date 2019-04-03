#pragma once

#include <vector>
#include <memory>
#include <spob/spob.h>
#include <spob/spob2glm.h>

namespace ftpip
{
	class Check;
	class TreeElem;

	typedef std::shared_ptr<Check> Check_ptr;
	typedef std::shared_ptr<TreeElem> TreeElem_ptr;

	/** Класс проверки принадлежности точки некоторому множеству. */
	class Check
	{
	public:
		~Check() {}

		virtual bool isInside(const spob::vec2& p) = 0;

		virtual double getComplexity(void) const = 0;
	protected:
		struct Compare1
		{
			bool less;
			double value;

			Compare1() : less(false), value(0) {}
			Compare1(bool less, double value) : less(less), value(value) {}

			bool check(double x) {
				if (less)
					return x < value;
				else
					return x > value;
			}
		};

		struct Compare2
		{
			bool less;
			double k, b;

			Compare2() : less(false), k(0), b(0) {}
			Compare2(bool less, double k, double b) : less(less), k(k), b(b) {}

			bool check(double x, double y) {
				if (less)
					return y - k*x - b < 0;
				else
					return y - k*x - b > 0;
			}
		};
	};

	/** Четырехугольник, стороны которого параллельны осям координат. */
	class QuadCheck : public Check
	{
	public:
		spob::vec2 up_left, down_right;

		// В прошлый раз не было проверок на принадлежность некоторому четырехугольнику, либо они были, но произошла трансформация координат, и все эти данные потеряны
		QuadCheck(const spob::vec2& up_left, const spob::vec2& down_right);

		// Считается, что в прошлый раз уже были проверки на принадлежность некоторому квадрату, и в этот раз проверки будут делаться так, чтоби не учитывать уже имеющуюся информацию 
		QuadCheck(const spob::vec2& up_left, const spob::vec2& down_right, 
		          const spob::vec2& last_up_left, const spob::vec2& last_down_right);

		~QuadCheck() {}

		bool isInside(const spob::vec2& p);

		double getComplexity(void) const;
	private:
		std::vector<Compare1> x_check;
		std::vector<Compare1> y_check;
	};

	/** Треугольник, образующийся на какой-нибудь диагонали Quad'а. */
	class HalfQuadCheck : public QuadCheck
	{
	public:
		// Передаются три точки треугольника, принадлежащего полигону. Треугольник обязательно должен быть прямоугольным, и все точки должны находиться на вершинах некоторого прямоугольника со сторонами параллельными осям координат
		HalfQuadCheck(const spob::vec2& a, 
		              const spob::vec2& b, 
		              const spob::vec2& c);

		// Аналогично предыдущему классу, учитываются предыдущие сравнения
		HalfQuadCheck(const spob::vec2& a, 
		              const spob::vec2& b, 
		              const spob::vec2& c,
		              const spob::vec2& last_up_left, spob::vec2& last_down_right);

		~HalfQuadCheck() {}

		bool isInside(const spob::vec2& p);

		double getComplexity(void) const;
	private:
		Compare2 xy_check; // Для определения положения относительно диагональной прямой
	};

	struct TreeElem
	{
		enum TreeType
		{
			TRUE, // Любая точка в этом узле принадлежит многоугольнику
			FALSE, // Аналогично любая не принадлежит
			NEXT, // Тут решается переход на следующие узлы
		};

		TreeType type;

		Check_ptr check;

		// Преобразования координат, которые применяются к точке, которая будет проходить через этот узел
		bool isTransform;
		glm::mat3 transform;

		TreeElem_ptr if_true; // Переход осуществляется на этот узел, если check выдало true
		TreeElem_ptr if_false; // Аналогично, если check выдало false
	};

	/*class Tree
	{
	public:
		Tree(const std::vector<vec2>& points);
		Tree(TreeElem_ptr tree);

		bool isInside(const spob::vec2& p) const;

		TreeElem_ptr tree;
	};*/

	double calcComplexity(TreeElem_ptr tree);

	void makeTree(TreeElem_ptr tree, const std::vector<spob::vec2>& points);

	bool isInside(TreeElem_ptr tree, const spob::vec2& p);
};