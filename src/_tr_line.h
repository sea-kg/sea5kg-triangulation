//---------------------------------------------------------------------------

#ifndef _tr_lineH
#define _tr_lineH

#include "_tr_point.h"

namespace triangulation {

point getNextPoint(point p1, point p2, double length)
{
	triangulation::point p3(0,0);
	double x1(p1.X), x2(p2.X), y1(p1.Y), y2(p2.Y);
	double del = sqrt( (x2-x1)*(x2-x1) + (y2 - y1)*(y2 - y1) );

	if(del == 0)
		return p3;

	p3.X = x1 + length * (x2-x1) / del;
	p3.Y = y1 + length * (y2-y1) / del;

	return p3;
};
//---------------------------------------------------------------------------

class line
{
	public:
		line() {}
		line(triangulation::point p1, triangulation::point p2)
			:   p1(p1), p2(p2)

		{

		};

		bool hasPoint(const triangulation::point &p3);
		const double getPerpendicularToLine(const triangulation::point &p3, triangulation::point &res);

		triangulation::point p1, p2;
		bool hasIntersection(triangulation::line &l, triangulation::point &result);
		double length() const 
		{
			double p12_X = (p1.X - p2.X);
			double p12_Y = (p1.Y - p2.Y);
			return sqrt( p12_X * p12_X + p12_Y * p12_Y);
		}

		triangulation::point getMiddlePoint() const
		{
			triangulation::point p;
			p.X = (p2.X + p1.X) / 2;
			p.Y = (p2.Y + p1.Y) / 2;

//			p.X = (p1.X < p2.X) ? (p2.X - p1.X) / 2 : (p1.X - p2.X) / 2;
//			p.Y = (p1.Y < p2.Y) ? (p2.Y - p1.Y) / 2 : (p1.Y - p2.Y) / 2;
			return p;
		};

		void paint(TImage *img)
		{
			img->Canvas->MoveTo(p1.X, p1.Y);
			img->Canvas->LineTo(p2.X, p2.Y);
			p1.paint(img);
			p2.paint(img);
		};

		private:
          bool hasIntersection2(triangulation::line &l, triangulation::point &result);

};

} // namespace triangulation
//---------------------------------------------------------------------------
#endif
