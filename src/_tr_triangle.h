//---------------------------------------------------------------------------

#ifndef _tr_triangleH
#define _tr_triangleH

#include "_tr_point.h"
#include "_tr_line.h"
//---------------------------------------------------------------------------

namespace triangulation {

class triangle
{
	public:
		triangle();
		triangle(triangulation::point p1, triangulation::point p2, triangulation::point p3);

		triangulation::point p1, p2, p3;

		double getSquare();
		bool findNearPoint(triangulation::point p, triangulation::point &res, double r);
		void paint(TImage *img);
		bool hasIntersection(const triangulation::line &l);
		bool hasPoint(const triangulation::point &p);
		bool hasTop(const triangulation::point &p);
		bool operator == (const triangulation::triangle &tr);
		void operator = (const triangulation::triangle &tr);

	private:
};
//---------------------------------------------------------------------------

}; // namespace triangulation
#endif
