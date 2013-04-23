//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_line.h"
#include <algorithm>
//---------------------------------------------------------------------------
namespace triangulation {

bool line::hasPoint(const triangulation::point &p3)
{
    triangulation::point result;
	double len = getPerpendicularToLine(p3, result);
	double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y);

	return (	len <= 1
		&& p3.X <= std::max(x2,x1) && p3.X >= std::min(x2,x1)
		&& p3.Y <= std::max(y2,y1) && p3.Y >= std::min(y2,y1)
	);

	return false;
};

const double line::getPerpendicularToLine(const triangulation::point &p3, triangulation::point &res)
{
	double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y), x3(p3.X), y3(p3.Y);

	if(x2 == x1)
	{
		res.X = x1;
		res.Y = p3.Y;
		return triangulation::line(p3, res).length();
	};

	if(y2 == y1)
	{
		res.X = p3.X;
		res.Y = y1;
		return triangulation::line(p3, res).length();
	};

	double k = (y2 - y1) / (x2 - x1);
	double b = (x2 * y1 - x1 * y2) / (x2 - x1);
	double k2 = -1 / k;
	double b2 = y3 - k2 * x3;

	res.X = (b2 - b) / (k - k2);
	res.Y = res.X * k2 + b2;

	return triangulation::line(p3, res).length();
};


bool line::hasIntersection(triangulation::line &l, triangulation::point &result)
{
	if(hasIntersection2(l, result))
		return true;
	else if(l.hasIntersection2(*this, result))
		return true;
	return false;
};

bool line::hasIntersection2(triangulation::line &l, triangulation::point &result)
{
   /*	if(getKUngle() == l.getKUngle())
		return false;*/

	double	dx1 = p2.X - p1.X;
	double	dy1 = p2.Y - p1.Y;
	double	dx2 = l.p2.X - l.p1.X;
	double	dy2 = l.p2.Y - l.p1.Y;
	result.X = dy1 * dx2 - dy2 * dx1;
	if(result.X == 0 || dx2 == 0)
		return false;

	result.Y = l.p1.X * l.p2.Y - l.p1.Y * l.p2.X;
	result.X = ((p1.X * p2.Y - p1.Y * p2.X) * dx2 - result.Y * dx1) / result.X;
	result.Y = (dy2 * result.X - result.Y) / dx2;

	return
		(
			(p1.X <= result.X && p2.X >= result.X)
			|| (p2.X <= result.X && p1.X >= result.X)
		)
		&&
		(
			(l.p1.X <= result.X && l.p2.X >= result.X)
			|| (l.p2.X <= result.X && l.p1.X >= result.X)
		);
};

} // namespace triangulation
#pragma package(smart_init)
