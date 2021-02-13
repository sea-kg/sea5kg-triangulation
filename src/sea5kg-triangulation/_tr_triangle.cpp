#include "_tr_triangle.h"

#include <vector>
#include <algorithm>
#include <math.h>
#include "_tr_area.h"

namespace triangulation {

triangle::triangle()
{
};

//---------------------------------------------------------------------------

triangle::triangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3)
    : p1(p1), p2(p2), p3(p3)
{

};

//---------------------------------------------------------------------------

double triangle::getSquare()
{
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

bool triangle::findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r) {
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

/*void triangle::paint(TImage *img)
{
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

bool triangle::hasIntersection(const Sea5kgTriangulationLine &l)
{
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

bool _sort_here (double x1, double x2) { return (x1 < x2); };

bool triangle::hasPoint(const Sea5kgTriangulationPoint &p)
{
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
    if(line12.hasIntersection(line_x, p_res))
      points_x.push_back(p_res.X);
    if(line23.hasIntersection(line_x, p_res))
      points_x.push_back(p_res.X);
    if(line31.hasIntersection(line_x, p_res))
      points_x.push_back(p_res.X);

    if(line12.hasIntersection(line_y, p_res))
        points_y.push_back(p_res.Y);
    if(line23.hasIntersection(line_y, p_res))
        points_y.push_back(p_res.Y);
    if(line31.hasIntersection(line_y, p_res))
        points_y.push_back(p_res.Y);

    points_x.push_back(1000);
    points_y.push_back(1000);

    std::sort (points_x.begin(), points_x.end(), _sort_here);
    std::sort (points_y.begin(), points_y.end(), _sort_here);

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
//---------------------------------------------------------------------------

bool triangle::hasTop(const Sea5kgTriangulationPoint &p) const
{
    double k = 1;
    return ((p1.calcLength(p) < k) || (p2.calcLength(p) < k) || (p3.calcLength(p) < k))
        || p1 == p || p2 == p || p3 == p;
};

//---------------------------------------------------------------------------

bool triangle::isEqual(const triangulation::triangle &tr)
{
    return hasTop(tr.p1) && hasTop(tr.p2) && hasTop(tr.p3);
};

//---------------------------------------------------------------------------

bool triangle::operator == (const triangulation::triangle &tr)
{
    return isEqual(tr);
};

//---------------------------------------------------------------------------

// UnicodeString triangle::toString()
// {
//     return "{" + p1.toString() + "," + p2.toString() + "," + p3.toString() + "}";
// };

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

void triangle::operator = (const triangulation::triangle &tr)
{
    p1 = tr.p1;
    p2 = tr.p2;
    p3 = tr.p3;
};

//---------------------------------------------------------------------------

triangle_pointer::triangle_pointer()
 : p1(new Sea5kgTriangulationPoint()), 
   p2(new Sea5kgTriangulationPoint()),
   p3(new Sea5kgTriangulationPoint())
{

};

//---------------------------------------------------------------------------

triangulation::triangle triangle_pointer::getTriangle()
{
  return triangulation::triangle(get_p1(), get_p2(), get_p3());
};

//---------------------------------------------------------------------------

Sea5kgTriangulationPoint triangle_pointer::get_p1()
{
  return Sea5kgTriangulationPoint(p1->X, p1->Y);
};

//---------------------------------------------------------------------------

Sea5kgTriangulationPoint triangle_pointer::get_p2()
{
  return Sea5kgTriangulationPoint(p2->X, p2->Y);
};

//---------------------------------------------------------------------------

Sea5kgTriangulationPoint triangle_pointer::get_p3()
{
  return Sea5kgTriangulationPoint(p3->X, p3->Y);
};

//---------------------------------------------------------------------------

};