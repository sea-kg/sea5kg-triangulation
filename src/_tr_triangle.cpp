//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_triangle.h"

namespace triangulation {

triangle::triangle()
{
};

//---------------------------------------------------------------------------

triangle::triangle(triangulation::point p1, triangulation::point p2, triangulation::point p3)
	: p1(p1), p2(p2), p3(p3)
{

};

//---------------------------------------------------------------------------

double triangle::getSquare()
{
	// formule Gerona for square of triangle use half premeter:
	double a = triangulation::line(p1, p2).length();
	double b = triangulation::line(p2, p3).length();
	double c = triangulation::line(p3, p1).length();
	double p = (a + b + c) / 2;
	return sqrt(p * (p - a) * (p - b) * (p - c));
};

//---------------------------------------------------------------------------

bool triangle::findNearPoint(triangulation::point p, triangulation::point &res, double r)
{
	res = p;
	if(p1.length(p) <= r)
		res = p1;
	else if(p2.length(p) <= r)
		res = p2;
	else if(p3.length(p) <= r)
		res = p3;
	return (res != p);
};

//---------------------------------------------------------------------------

void triangle::paint(TImage *img)
{
	TColor nPenColor = img->Canvas->Pen->Color;
	TColor nBrushColor = img->Canvas->Brush->Color;
	int nWidth = img->Canvas->Pen->Width;

	img->Canvas->Pen->Color = clRed;
	img->Canvas->Pen->Width = 2;
	img->Canvas->Brush->Color = clSilver;
/*	TColor newColor = random(256);
	newColor = (newColor << 8) + random(256);
	newColor = (newColor << 8) + random(256);
	img->Canvas->Brush->Color =  newColor;*/

	TPoint points[3];
	points[0] = Point(p1.X,p1.Y);
	points[1] = Point(p2.X,p2.Y);
	points[2] = Point(p3.X,p3.Y);
	img->Canvas->Polygon(points, 2);

/*	triangulation::line(p1, p2).paint(img);
	triangulation::line(p2, p3).paint(img);
	triangulation::line(p3, p1).paint(img);*/

	img->Canvas->Pen->Color = nPenColor;
	img->Canvas->Brush->Color = nBrushColor;
	img->Canvas->Pen->Width = nWidth;

	p1.paint(img);
	p2.paint(img);
	p3.paint(img);
};

//---------------------------------------------------------------------------

bool triangle::hasIntersection(const triangulation::line &l)
{
	triangulation::point p;
	triangulation::line l0(l.p1,l.p2);
	triangulation::line l12(p1,p2);
	triangulation::line l23(p2,p3);
	triangulation::line l31(p3,p1);

	return
		   l12.hasIntersection(l0, p) || l0.hasIntersection(l12, p)
		|| l23.hasIntersection(l0, p) || l0.hasIntersection(l23, p)
		|| l31.hasIntersection(l0, p) || l0.hasIntersection(l31, p);
};

//---------------------------------------------------------------------------

bool triangle::hasTop(const triangulation::point &p)
{
	double k = 1;
	return (p1.length(p) < k) || (p2.length(p) < k) || (p3.length(p) < k);
};

//---------------------------------------------------------------------------

bool triangle::operator == (const triangulation::triangle &tr)
{
	return hasTop(tr.p1) && hasTop(tr.p2) && hasTop(tr.p3);
};

//---------------------------------------------------------------------------

void triangle::operator = (const triangulation::triangle &tr)
{
	p1 = tr.p1;
	p2 = tr.p2;
	p3 = tr.p3;
};

};

//---------------------------------------------------------------------------


#pragma package(smart_init)
