#pragma once

#include "_tr_point.h"
#include "_tr_line.h"
#include "_tr_area.h"
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
        // void paint(TImage *img);
        bool hasIntersection(const triangulation::line &l);
        bool hasPoint(const triangulation::point &p);
        bool hasTop(const triangulation::point &p) const;
        // UnicodeString toString();
        // void triangle::fromString(UnicodeString str);
        bool operator == (const triangulation::triangle &tr);
        void operator = (const triangulation::triangle &tr);
        bool isEqual(const triangulation::triangle &tr);

    private:
};
//---------------------------------------------------------------------------

class triangle_pointer
{
    public:
        triangle_pointer();

        triangulation::point* p1;
    triangulation::point* p2;
    triangulation::point* p3;

        triangulation::triangle getTriangle();
    triangulation::point get_p1();
    triangulation::point get_p2();
    triangulation::point get_p3();

    private:
};
//---------------------------------------------------------------------------

}; // namespace triangulation

