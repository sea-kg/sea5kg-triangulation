//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_area.h"
#include <algorithm>
#include <math.h>

namespace triangulation {

bool _sort_here (double x1, double x2) { return (x1 < x2); };

bool area::hasPoint(const triangulation::point &p) const
{

	triangulation::line line_x( triangulation::point(-1000, p.Y), triangulation::point(1000, p.Y) );
	triangulation::line line_y( triangulation::point(p.X, -1000), triangulation::point(p.X,1000) );

	std::vector<double> points_x;
	std::vector<double> points_y;
	points_x.push_back(-1000);
	points_y.push_back(-1000);

	for(int i = 0; i < m_vPoints.size(); i++)
	{
		triangulation::point p1 = m_vPoints[i];
		triangulation::point p2 = m_vPoints[(i+1) % m_vPoints.size()];

		triangulation::line line_side(p1,p2);

		triangulation::point p_res;
		if(line_side.hasIntersection(line_x, p_res))
		  points_x.push_back(p_res.X);
		else if(line_x.hasIntersection(line_side, p_res))
		  points_x.push_back(p_res.X);

		if(line_side.hasIntersection(line_y, p_res))
		  points_y.push_back(p_res.Y);
		else if(line_y.hasIntersection(line_side, p_res))
		  points_y.push_back(p_res.Y);
	}

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

	return bX && bY;
};
//---------------------------------------------------------------------------

bool area::hasIntersections(triangulation::line l, std::vector<triangulation::point> &result)
{
	int nCount = 0;
	for(int i = 0; i < this->count(); i++)
	{
		triangulation::line line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));

		triangulation::point p_res;
		if(l.hasIntersection(line_side, p_res))
		{
			nCount++;
			result.push_back(p_res);
		}
	}
	return (nCount > 0);
};
//---------------------------------------------------------------------------

bool area::hasIntersections(triangulation::area &ar, std::vector<triangulation::point> &result)
{
	int nCount = 0;
	for(int ar_i = 0; ar_i < ar.count(); ar_i++)
	{
		triangulation::point ar_p1 = ar.getPoint(ar_i);
		triangulation::point ar_p2 = ar.getPoint((ar_i+1) % ar.count());

		triangulation::line ar_line(ar_p1, ar_p2);


		for(int i = 0; i < this->count(); i++)
		{
			triangulation::line line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));
			triangulation::point p_res;
			if(line_side.hasIntersection(ar_line, p_res))
			{
				nCount++;
                result.push_back(p_res);
			};
		}
	}
	return (nCount > 0);
};
//---------------------------------------------------------------------------

void area::paint(TImage *img)
{
	TColor nColor = img->Canvas->Pen->Color;
	int nWidth = img->Canvas->Pen->Width;

	img->Canvas->Pen->Color = clBlue;
	img->Canvas->Pen->Width = 2;

	if(count() > 0 )
	{
		triangulation::point p = getPoint(0);
		img->Canvas->MoveTo(p.X,p.Y);
		img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
	}

	for(int i = 1; i < count(); i++)
	{
	  triangulation::point p = getPoint(i);
	  img->Canvas->LineTo(p.X,p.Y);
	  img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
	};

	if(count() > 0 )
	{
	  triangulation::point p = getPoint(0);
	  img->Canvas->LineTo(p.X,p.Y);
	};

	img->Canvas->Pen->Color = nColor;
	img->Canvas->Pen->Width = nWidth;

};

//---------------------------------------------------------------------------

double area::getSquare()
{
	triangulation::point pMiddlePoint = getMiddlePoint();
	double square = 0;
	for(int i = 0; i < count(); i++)
	{
		triangulation::point p1 = getPoint(i);
		triangulation::point p2 = getPoint((i+1) % count());
		square += triangulation::triangle(p1, p2, pMiddlePoint).getSquare();
	};
	return square;
};

//---------------------------------------------------------------------------

triangulation::point area::getMiddlePoint()
{
	triangulation::point pMiddlePoint;
	for(int i = 0; i < count(); i++)
		pMiddlePoint += getPoint(i);
	pMiddlePoint = pMiddlePoint / count();
	return pMiddlePoint;
};
//---------------------------------------------------------------------------

bool area::findNearPointSide(const triangulation::point &p, triangulation::point &result, double len)
{
	for(int i = 0; i < count(); i++)
	{
		triangulation::point p1 = getPoint(i);
		if(p1.length(p) < len)
		{
			result = p1;
			return true;
		};
	};

	triangulation::point res2;
	double nLen2 = len;

	for(int i = 0; i < count(); i++)
	{
		triangulation::point p1 = getPoint(i);
		triangulation::point p2 = getPoint((i+1) % count());
		triangulation::line L(p1, p2);
		triangulation::point res;
		double nLen = L.getPerpendicularToLine(p, res);
		if( nLen < nLen2 && L.hasPoint(res))
		{
			res2 = res;
			nLen2 = nLen;
		};
	};

	if(nLen2 < len)
	{
		result = res2;
		return true;
	};

	return false;
};

}; // namespace triangulation



//---------------------------------------------------------------------------
#pragma package(smart_init)
