#pragma once

#include "_tr_point.h"
#include <cmath>

namespace triangulation {

point getNextPoint(point p1, point p2, double length);

//---------------------------------------------------------------------------

class line {
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
        double length() const;
        triangulation::point getMiddlePoint() const
        {
            triangulation::point p;
            p.X = (p2.X + p1.X) / 2;
            p.Y = (p2.Y + p1.Y) / 2;

//            p.X = (p1.X < p2.X) ? (p2.X - p1.X) / 2 : (p1.X - p2.X) / 2;
//            p.Y = (p1.Y < p2.Y) ? (p2.Y - p1.Y) / 2 : (p1.Y - p2.Y) / 2;
            return p;
        };

        /*void paint(TImage *img)
        {
            img->Canvas->MoveTo(p1.X, p1.Y);
            img->Canvas->LineTo(p2.X, p2.Y);
            p1.paint(img);
            p2.paint(img);
        };*/

        private:
            bool hasIntersection2(triangulation::line &l, triangulation::point &result);
            bool hasIntersection3(triangulation::line &l, triangulation::point &result);

};

} // namespace triangulation
