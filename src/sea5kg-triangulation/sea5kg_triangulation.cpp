#include "sea5kg_triangulation.h"
#include <algorithm>
#include <cmath>

//---------------------------------------------------------------------------
// Sea5kgTriangulationPoint

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint()
: X(0), Y(0), m_nCounter(0) {

};

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint(double X, double Y) 
: X(X), Y(Y), m_nCounter(0) {

};

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint(const Sea5kgTriangulationPoint &p)
: X(p.X), Y(p.Y), m_nCounter(0) {

};

void Sea5kgTriangulationPoint::incCounter() {
    m_nCounter++;
};

int Sea5kgTriangulationPoint::getCounterValue() {
    return m_nCounter;
};

const double &Sea5kgTriangulationPoint::getX() {
    return X;
}

const double &Sea5kgTriangulationPoint::getY() {
    return Y;
}

void Sea5kgTriangulationPoint::operator+=(const Sea5kgTriangulationPoint &p) {
    X += p.X;
    Y += p.Y;
};

const Sea5kgTriangulationPoint Sea5kgTriangulationPoint::operator / (const int n) {
    Sea5kgTriangulationPoint res_p;

    if (X != 0 && Y != 0) {
        res_p.X += X / n;
        res_p.Y += Y / n;
    }
    return res_p;
};

double Sea5kgTriangulationPoint::calcLength(const Sea5kgTriangulationPoint &p) const {
    double p12_X = (X - p.X);
    double p12_Y = (Y - p.Y);
    return sqrt( p12_X * p12_X + p12_Y * p12_Y);
};

void Sea5kgTriangulationPoint::operator = (const Sea5kgTriangulationPoint &p) {
    X = p.X;
    Y = p.Y;
};

bool Sea5kgTriangulationPoint::operator == (const Sea5kgTriangulationPoint &p) const {
    return (p.X == X) && (p.Y == Y);
};

bool Sea5kgTriangulationPoint::operator != (const Sea5kgTriangulationPoint &p) {
    return (p.X != X) || (p.Y != Y);
};

std::string Sea5kgTriangulationPoint::toString() {
    return "(" + std::to_string(int(X)) + "," + std::to_string(int(Y)) + ")";
};


Sea5kgTriangulationPoint getNextPoint(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, double length) {
    Sea5kgTriangulationPoint p3(0,0);
    double x1(p1.X), x2(p2.X), y1(p1.Y), y2(p2.Y);
    double del = sqrt( (x2-x1)*(x2-x1) + (y2 - y1)*(y2 - y1) );

    if(del == 0)
        return p3;

    p3.X = x1 + length * (x2-x1) / del;
    p3.Y = y1 + length * (y2-y1) / del;

    return p3;
}

//---------------------------------------------------------------------------
// Sea5kgTriangulationLine

Sea5kgTriangulationLine::Sea5kgTriangulationLine() {

}

Sea5kgTriangulationLine::Sea5kgTriangulationLine(
    Sea5kgTriangulationPoint p1, 
    Sea5kgTriangulationPoint p2
) {
    this->p1 = p1; 
    this->p2 = p2;
};

bool Sea5kgTriangulationLine::hasPoint(const Sea5kgTriangulationPoint &p3)
{
    Sea5kgTriangulationPoint result;
    double len = getPerpendicularToLine(p3, result);
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y);

    return (    len <= 1
        && p3.X <= std::max(x2,x1) && p3.X >= std::min(x2,x1)
        && p3.Y <= std::max(y2,y1) && p3.Y >= std::min(y2,y1)
    );

    return false;
};

//---------------------------------------------------------------------------

double Sea5kgTriangulationLine::length() const 
{
    double p12_X = (p1.X - p2.X);
    double p12_Y = (p1.Y - p2.Y);
    return sqrt( p12_X * p12_X + p12_Y * p12_Y);
}

//---------------------------------------------------------------------------

const double Sea5kgTriangulationLine::getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res)
{
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y), x3(p3.X), y3(p3.Y);

    if(x2 == x1)
    {
        res.X = x1;
        res.Y = p3.Y;
        return Sea5kgTriangulationLine(p3, res).length();
    };

    if(y2 == y1)
    {
        res.X = p3.X;
        res.Y = y1;
        return Sea5kgTriangulationLine(p3, res).length();
    };

    double k = (y2 - y1) / (x2 - x1);
    double b = (x2 * y1 - x1 * y2) / (x2 - x1);
    double k2 = -1 / k;
    double b2 = y3 - k2 * x3;

    res.X = (b2 - b) / (k - k2);
    res.Y = res.X * k2 + b2;

    return Sea5kgTriangulationLine(p3, res).length();
};

bool Sea5kgTriangulationLine::hasIntersection(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    if(hasIntersection3(l, result))
        return true;
    else if(l.hasIntersection3(*this, result))
        return true;
    return false;
};

Sea5kgTriangulationPoint Sea5kgTriangulationLine::getMiddlePoint() const {
    Sea5kgTriangulationPoint p;
    p.X = (p2.X + p1.X) / 2;
    p.Y = (p2.Y + p1.Y) / 2;
    // p.X = (p1.X < p2.X) ? (p2.X - p1.X) / 2 : (p1.X - p2.X) / 2;
    // p.Y = (p1.Y < p2.Y) ? (p2.Y - p1.Y) / 2 : (p1.Y - p2.Y) / 2;
    return p;
};

bool Sea5kgTriangulationLine::hasIntersection2(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    // if(getKUngle() == l.getKUngle()) {
    //    return false;
    // }
    // TODO: this function incorrect for vertical lines
    double dx1 = p2.X - p1.X;
    double dy1 = p2.Y - p1.Y;
    double dx2 = l.p2.X - l.p1.X;
    double dy2 = l.p2.Y - l.p1.Y;
    result.X = dy1 * dx2 - dy2 * dx1;
    if(result.X == 0 || dx2 == 0)
        return false;

    result.Y = l.p1.X * l.p2.Y - l.p1.Y * l.p2.X;
    result.X = ((p1.X * p2.Y - p1.Y * p2.X) * dx2 - result.Y * dx1) / result.X;
    result.Y = (dy2 * result.X - result.Y) / dx2;

    return  hasPoint(result) && l.hasPoint(result);
//        (
//            (p1.X <= result.X && p2.X >= result.X)
//            || (p2.X <= result.X && p1.X >= result.X)
//        )
//        &&
//        (
//            (l.p1.X <= result.X && l.p2.X >= result.X)
//            || (l.p2.X <= result.X && l.p1.X >= result.X)
//        );
};


bool Sea5kgTriangulationLine::hasIntersection3(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    // if(getKUngle() == l.getKUngle()) {
    //     return false;
    // }
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y), x3(l.p1.X), y3(l.p1.Y), x4(l.p2.X), y4(l.p2.Y);

    if (x2 == x1 && x4 == x3) {
        return false;
    } else if (x2 == x1 && x4 != x3) {
        double k = (y4 - y3) / (x4 - x3);
        double b = (x4 * y3 - x3 * y4) / (x4 - x3);
        result.X = x1;
        result.Y = result.X * k + b;
        return  hasPoint(result) && l.hasPoint(result);
    } else if (x2 != x1 && x4 == x3) {
        double k = (y2 - y1) / (x2 - x1);
        double b = (x2 * y1 - x1 * y2) / (x2 - x1);
        result.X = x1;
        result.Y = result.X * k + b;
        return  hasPoint(result) && l.hasPoint(result);
    }

    double dx1 = x2 - x1;
    double dy1 = y2 - y1;
    double dx2 = x4 - x3;
    double dy2 = y4 - y3;
    result.X = dy1 * dx2 - dy2 * dx1;

    if (result.X == 0) {
        return false;
    }

    result.Y = x3 * y4 - y3 * x4;
    result.X = ((x1 * y2 - y1 * x2) * dx2 - result.Y * dx1) / result.X;
    result.Y = (dy2 * result.X - result.Y) / dx2;

    return hasPoint(result) && l.hasPoint(result);
};

