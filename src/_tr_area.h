//---------------------------------------------------------------------------

#ifndef _tr_areaH
#define _tr_areaH

#include <vector>
#include <math.h>
#include <vcl.h>
#include "_tr_point.h"
#include "_tr_line.h"
#include "_tr_triangle.h"

namespace triangulation {

class area
{
	public:
		area() {};
		void addPoint(int X, int Y) { m_vPoints.push_back(triangulation::point(X,Y)); };
		void clear() { m_vPoints.clear(); };
		int count() const { return m_vPoints.size(); };
		const triangulation::point &getPoint(int i) const { return m_vPoints[i]; };
		void setCountTriangles(int nCountTriangles)  { m_nCountTriangles = nCountTriangles; };
		unsigned int getCountTriangles() const { return m_nCountTriangles; };
		bool hasPoint(const triangulation::point &p1) const;
		const double getPerpendicularToLine(const triangulation::point &p3, triangulation::point &res);
		bool hasLine(const triangulation::line &l) const;
		bool hasIntersections(triangulation::line l, std::vector<triangulation::point> &result);
		bool hasIntersections(triangulation::area &l, std::vector<triangulation::point> &result);
		void paint(TImage *img);
		double getSquare();
		triangulation::point getMiddlePoint();
		bool findNearPointSide(const triangulation::point &p, triangulation::point &res, double len);
    bool getMinMaxXY(double &minX, double &maxX, double &minY, double &maxY);
	private:
		std::vector<triangulation::point> m_vPoints;
		unsigned int m_nCountTriangles;
};

}; //  namespace triangulation

//---------------------------------------------------------------------------
#endif
