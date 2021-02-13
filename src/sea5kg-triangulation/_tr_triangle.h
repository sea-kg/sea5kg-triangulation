#pragma once

#include "sea5kg_triangulation.h"


namespace triangulation {

class triangle
{
    public:
        triangle();
        triangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3);

        Sea5kgTriangulationPoint p1, p2, p3;

        double getSquare();
        bool findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r);
        // void paint(TImage *img);
        bool hasIntersection(const Sea5kgTriangulationLine &l);
        bool hasPoint(const Sea5kgTriangulationPoint &p);
        bool hasTop(const Sea5kgTriangulationPoint &p) const;
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

        Sea5kgTriangulationPoint* p1;
    Sea5kgTriangulationPoint* p2;
    Sea5kgTriangulationPoint* p3;

        triangulation::triangle getTriangle();
    Sea5kgTriangulationPoint get_p1();
    Sea5kgTriangulationPoint get_p2();
    Sea5kgTriangulationPoint get_p3();

    private:
};
//---------------------------------------------------------------------------

}; // namespace triangulation

