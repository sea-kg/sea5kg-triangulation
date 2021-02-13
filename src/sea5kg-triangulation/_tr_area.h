#pragma once

#include <vector>
#include <math.h>
#include "_tr_triangle.h"

namespace triangulation {

class area
{
    public:
        area() {};
        void addPoint(int X, int Y) { m_vPoints.push_back(Sea5kgTriangulationPoint(X,Y)); };
        void clear() { m_vPoints.clear(); };
        int count() const { return m_vPoints.size(); };
        const Sea5kgTriangulationPoint &getPoint(int i) const { return m_vPoints[i]; };
        void setCountTriangles(int nCountTriangles)  { m_nCountTriangles = nCountTriangles; };
        unsigned int getCountTriangles() const { return m_nCountTriangles; };
        bool hasPoint(const Sea5kgTriangulationPoint &p1) const;
        const double getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res);
        bool hasLine(const Sea5kgTriangulationLine &l) const;
        bool hasIntersections(Sea5kgTriangulationLine l, std::vector<Sea5kgTriangulationPoint> &result);
        bool hasIntersections(triangulation::area &l, std::vector<Sea5kgTriangulationPoint> &result);
        // void paint(TImage *img);
        double getSquare();
        Sea5kgTriangulationPoint getMiddlePoint();
        bool findNearPointSide(const Sea5kgTriangulationPoint &p, Sea5kgTriangulationPoint &res, double len);
    bool getMinMaxXY(double &minX, double &maxX, double &minY, double &maxY);
    private:
        std::vector<Sea5kgTriangulationPoint> m_vPoints;
        unsigned int m_nCountTriangles;
};

}; //  namespace triangulation
