//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_point.h"
#include <cmath>

namespace triangulation {

point::point()
    : X(0), Y(0), m_nCounter(0)
{

};

//---------------------------------------------------------------------------

point::point(double X, double Y) : X(X), Y(Y), m_nCounter(0)
{

};

//---------------------------------------------------------------------------

point::point(const triangulation::point &p) : X(p.X), Y(p.Y), m_nCounter(0)
{

};

//---------------------------------------------------------------------------

void point::incCounter()
{
    m_nCounter++;
};

//---------------------------------------------------------------------------

int point::getCounterValue()
{
    return m_nCounter;
};

//---------------------------------------------------------------------------

// void point::paint(TImage *img)
// {
//     img->Canvas->Ellipse(X + 2, Y + 2, X - 2, Y - 2);
// };

//---------------------------------------------------------------------------

void point::operator += (const triangulation::point &p)
{
    X += p.X;
    Y += p.Y;
};

//---------------------------------------------------------------------------

const triangulation::point point::operator / (const int n)
{
    triangulation::point res_p;

    if(X != 0 && Y != 0)
    {
        res_p.X += X / n;
        res_p.Y += Y / n;
    }
    return res_p;
};

//---------------------------------------------------------------------------

double point::length(const triangulation::point &p) const
{
    double p12_X = (X - p.X);
    double p12_Y = (Y - p.Y);
    return sqrt( p12_X * p12_X + p12_Y * p12_Y);
};

//---------------------------------------------------------------------------

void point::operator = (const triangulation::point &p)
{
    X = p.X;
    Y = p.Y;
};

//---------------------------------------------------------------------------

bool point::operator == (const triangulation::point &p) const
{
    return (p.X == X) && (p.Y == Y);
};

//---------------------------------------------------------------------------

bool point::operator != (const triangulation::point &p)
{
    return (p.X != X) || (p.Y != Y);
};

//---------------------------------------------------------------------------

// UnicodeString point::toString()
// {
//     return "(" + IntToStr(int(X)) + "," + IntToStr(int(Y)) + ")";
// };

}; // triangulation

//---------------------------------------------------------------------------
#pragma package(smart_init)
