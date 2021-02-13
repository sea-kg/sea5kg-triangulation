#include "sea5kg_triangulation.h"
#include <algorithm>
#include "_tr_area.h"

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


//---------------------------------------------------------------------------
// Sea5kgTriangulationTriangle

Sea5kgTriangulationTriangle::Sea5kgTriangulationTriangle() {
    // Nothing
};

//---------------------------------------------------------------------------

Sea5kgTriangulationTriangle::Sea5kgTriangulationTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3)
    : p1(p1), p2(p2), p3(p3)
{

};

//---------------------------------------------------------------------------

double Sea5kgTriangulationTriangle::getSquare() {
    // formule Gerona for square of triangle use half premeter:
    double a = Sea5kgTriangulationLine(p1, p2).length();
    double b = Sea5kgTriangulationLine(p2, p3).length();
    double c = Sea5kgTriangulationLine(p3, p1).length();
    double p = (a + b + c) / 2;
    double res = p * (p - a) * (p - b) * (p - c);
    if(res > 0.1) res = sqrt(res);
    return res;
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangle::findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r) {
    res = p;
    if (p1.calcLength(p) <= r) {
        res = p1;
    } else if(p2.calcLength(p) <= r) {
        res = p2;
    } else if(p3.calcLength(p) <= r) {
        res = p3;
    }
    return (res != p);
};

//---------------------------------------------------------------------------

/*void Sea5kgTriangulationTriangle::paint(TImage *img) {
    TColor nPenColor = img->Canvas->Pen->Color;
    TColor nBrushColor = img->Canvas->Brush->Color;
    int nWidth = img->Canvas->Pen->Width;

    img->Canvas->Pen->Color = clRed;
    img->Canvas->Pen->Width = 2;
    img->Canvas->Brush->Color = clSilver;

    TPoint points[3];
    points[0] = Point(p1.X,p1.Y);
    points[1] = Point(p2.X,p2.Y);
    points[2] = Point(p3.X,p3.Y);
    img->Canvas->Polygon(points, 2);

    img->Canvas->Pen->Color = nPenColor;
    img->Canvas->Brush->Color = nBrushColor;
    img->Canvas->Pen->Width = nWidth;

    p1.paint(img);
    p2.paint(img);
    p3.paint(img);
};*/

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangle::hasIntersection(const Sea5kgTriangulationLine &l) {
    Sea5kgTriangulationPoint p;
    Sea5kgTriangulationLine l0(l.p1,l.p2);
    Sea5kgTriangulationLine l12(p1,p2);
    Sea5kgTriangulationLine l23(p2,p3);
    Sea5kgTriangulationLine l31(p3,p1);

    return
           l12.hasIntersection(l0, p) || l0.hasIntersection(l12, p)
        || l23.hasIntersection(l0, p) || l0.hasIntersection(l23, p)
        || l31.hasIntersection(l0, p) || l0.hasIntersection(l31, p);
};

//---------------------------------------------------------------------------

bool _sort_here_triangle (double x1, double x2) { return (x1 < x2); };

bool Sea5kgTriangulationTriangle::hasPoint(const Sea5kgTriangulationPoint &p) {
    triangulation::area ar;        
    ar.addPoint(p1.X, p1.Y);
    ar.addPoint(p2.X, p2.Y);
    ar.addPoint(p3.X, p3.Y);
    return ar.hasPoint(p);

    Sea5kgTriangulationLine line_x( Sea5kgTriangulationPoint(-1000, p.Y), Sea5kgTriangulationPoint(1000, p.Y) );
    Sea5kgTriangulationLine line_y( Sea5kgTriangulationPoint(p.X, -1000), Sea5kgTriangulationPoint(p.X,1000) );

    std::vector<double> points_x;
    std::vector<double> points_y;
    points_x.push_back(-1000);
    points_y.push_back(-1000);


    Sea5kgTriangulationLine line12(p1,p2);
    Sea5kgTriangulationLine line23(p2,p3);
    Sea5kgTriangulationLine line31(p3,p2);

    Sea5kgTriangulationPoint p_res;
    if (line12.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }
    if (line23.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }
    if(line31.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }

    if (line12.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
    if (line23.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
    if (line31.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
        

    points_x.push_back(1000);
    points_y.push_back(1000);

    std::sort (points_x.begin(), points_x.end(), _sort_here_triangle);
    std::sort (points_y.begin(), points_y.end(), _sort_here_triangle);

    bool bX = false;
    for(int i = 0; i < points_x.size() -1; i++)
    {
        double x1 = points_x[i];
        double x2 = points_x[i+1];
        if((i+1) % 2 == 0)
        {
            if(x1 <= p.X && p.X <= x2)
                bX = true;
        };
    };

    bool bY = false;
    for(int i = 0; i < points_y.size() -1; i++)
    {
        double y1 = points_y[i];
        double y2 = points_y[i+1];
        if((i+1) % 2 == 0)
        {
            if(y1 <= p.Y && p.Y <= y2)
                bY = true;
        };
    };

    if( bX && bY 
            && line12.getPerpendicularToLine(p, p_res) > 2 
            && line23.getPerpendicularToLine(p, p_res) > 2
            && line31.getPerpendicularToLine(p, p_res) > 2
    )
    {
        return true;
    };
    
    return false; 
};

bool Sea5kgTriangulationTriangle::hasTop(const Sea5kgTriangulationPoint &p) const
{
    double k = 1;
    return ((p1.calcLength(p) < k) || (p2.calcLength(p) < k) || (p3.calcLength(p) < k))
        || p1 == p || p2 == p || p3 == p;
};

bool Sea5kgTriangulationTriangle::isEqual(const Sea5kgTriangulationTriangle &tr)
{
    return hasTop(tr.p1) && hasTop(tr.p2) && hasTop(tr.p3);
};

bool Sea5kgTriangulationTriangle::operator == (const Sea5kgTriangulationTriangle &tr)
{
    return isEqual(tr);
};

std::string Sea5kgTriangulationTriangle::toString() {
    return "{" + p1.toString() + "," + p2.toString() + "," + p3.toString() + "}";
};

// UnicodeString GetPart(UnicodeString str, UnicodeString from, UnicodeString to)
// {
//   str.Delete(1,str.Pos(from)-from.Length() + 1);
//   str = str.SubString(1,str.Pos(to)-1);
//   return str;
// }

// void setPoint(UnicodeString str, Sea5kgTriangulationPoint &p)
// {
//     p.X = StrToInt(str.SubString(1, str.Pos(",")-1));
//     str.Delete(1, str.Pos(","));
//     p.Y = StrToInt(str);
// };

// void triangle::fromString(UnicodeString str)
// {
//     str = GetPart(str, "{", "}");
//     UnicodeString str_p1 = GetPart(str, "(", ")");
//     str.Delete(1, str.Pos(")"));
//     UnicodeString str_p2 = GetPart(str, "(", ")");
//     str.Delete(1, str.Pos(")"));
//     UnicodeString str_p3 = GetPart(str, "(", ")");
// 
//     setPoint(str_p1, p1);
//     setPoint(str_p2, p2);
//     setPoint(str_p3, p3);
// 
//     // return "{ " + p1.toString() + "," + p2.toString() + "," + p3.toString() + "}";
// };

//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangle::operator = (const Sea5kgTriangulationTriangle &tr) {
    p1 = tr.p1;
    p2 = tr.p2;
    p3 = tr.p3;
};

//---------------------------------------------------------------------------
// Sea5kgTriangulationTrianglePointer

Sea5kgTriangulationTrianglePointer::Sea5kgTriangulationTrianglePointer() {
    p1 = new Sea5kgTriangulationPoint();
    p2 = new Sea5kgTriangulationPoint();
    p3 = new Sea5kgTriangulationPoint();
};

Sea5kgTriangulationTriangle Sea5kgTriangulationTrianglePointer::getTriangle() {
  return Sea5kgTriangulationTriangle(get_p1(), get_p2(), get_p3());
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p1() {
  return Sea5kgTriangulationPoint(p1->X, p1->Y);
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p2() {
  return Sea5kgTriangulationPoint(p2->X, p2->Y);
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p3() {
  return Sea5kgTriangulationPoint(p3->X, p3->Y);
};
