#pragma once

#include <cmath>
#include <string>
#include <vector>

class Sea5kgTriangulationPoint {
    public:
        Sea5kgTriangulationPoint();
        Sea5kgTriangulationPoint(double X, double Y);
        Sea5kgTriangulationPoint(const Sea5kgTriangulationPoint &p);
        void incCounter();
        int getCounterValue();

        const double &getX();
        const double &getY();
        double calcLength(const Sea5kgTriangulationPoint &p) const;

        void operator += (const Sea5kgTriangulationPoint &p);
        const Sea5kgTriangulationPoint operator / (const int n);
        void operator = (const Sea5kgTriangulationPoint &p);
        bool operator == (const Sea5kgTriangulationPoint &p) const;
        bool operator != (const Sea5kgTriangulationPoint &p);
        std::string toString();
        double X,Y;

    private:
        int m_nCounter;
};

Sea5kgTriangulationPoint getNextPoint(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, double length);

class Sea5kgTriangulationLine {
    public:
        Sea5kgTriangulationLine();
        Sea5kgTriangulationLine(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2);

        bool hasPoint(const Sea5kgTriangulationPoint &p3);
        const double getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res);

        Sea5kgTriangulationPoint p1, p2;
        bool hasIntersection(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
        double length() const;
        Sea5kgTriangulationPoint getMiddlePoint() const;

        private:
            bool hasIntersection2(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
            bool hasIntersection3(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
};

class Sea5kgTriangulationTriangle {
    public:
        Sea5kgTriangulationTriangle();
        Sea5kgTriangulationTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3);

        // TODO hide
        Sea5kgTriangulationPoint p1, p2, p3;

        double getSquare();
        bool findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r);
        // void paint(TImage *img);
        bool hasIntersection(const Sea5kgTriangulationLine &l);
        bool hasPoint(const Sea5kgTriangulationPoint &p);
        bool hasTop(const Sea5kgTriangulationPoint &p) const;
        std::string toString();
        // void triangle::fromString(UnicodeString str);
        bool operator == (const Sea5kgTriangulationTriangle &tr);
        void operator = (const Sea5kgTriangulationTriangle &tr);
        bool isEqual(const Sea5kgTriangulationTriangle &tr);

    private:
};

class Sea5kgTriangulationTrianglePointer {
    public:
        Sea5kgTriangulationTrianglePointer();

        Sea5kgTriangulationPoint* p1;
        Sea5kgTriangulationPoint* p2;
        Sea5kgTriangulationPoint* p3;

        Sea5kgTriangulationTriangle getTriangle();
        Sea5kgTriangulationPoint get_p1();
        Sea5kgTriangulationPoint get_p2();
        Sea5kgTriangulationPoint get_p3();

    private:
};