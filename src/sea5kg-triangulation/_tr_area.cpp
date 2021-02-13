//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_area.h"
#include <algorithm>
#include <math.h>

namespace triangulation {

bool _sort_here_area (double x1, double x2) {
    return (x1 < x2);
};

bool area::hasPoint(const Sea5kgTriangulationPoint &p) const
{
    double k = 2;
    Sea5kgTriangulationLine line_x( Sea5kgTriangulationPoint(-1000, p.Y), Sea5kgTriangulationPoint(1000, p.Y) );
    Sea5kgTriangulationLine line_y( Sea5kgTriangulationPoint(p.X, -1000), Sea5kgTriangulationPoint(p.X,1000) );

    std::vector<double> points_x;
    std::vector<double> points_y;
    points_x.push_back(-1000);
    points_y.push_back(-1000);

    for(int i = 0; i < m_vPoints.size(); i++)
    {
        Sea5kgTriangulationPoint p1 = m_vPoints[i];
        Sea5kgTriangulationPoint p2 = m_vPoints[(i+1) % m_vPoints.size()];

        Sea5kgTriangulationLine line_side(p2,p1);

        Sea5kgTriangulationPoint p_res;

        //if(line_x.hasIntersection(line_side, p_res)) // && line_x.hasPoint(p_res) && line_side.hasPoint(p_res) )
        //    points_x.push_back(p_res.X) ;

        if(line_side.hasIntersection(line_x, p_res))
            points_x.push_back(p_res.X);

        //if(line_y.hasIntersection(line_side, p_res)) // && line_y.hasPoint(p_res) && line_side.hasPoint(p_res) )
        //    points_y.push_back(p_res.Y);

        if(line_side.hasIntersection(line_y, p_res)) // && line_y.hasPoint(p_res) && line_side.hasPoint(p_res) )
            points_y.push_back(p_res.Y);


    }

    points_x.push_back(1000);
    points_y.push_back(1000);

    std::sort (points_x.begin(), points_x.end(), _sort_here_area);
    std::sort (points_y.begin(), points_y.end(), _sort_here_area);

    bool bX = false;
    for(int i = 0; i < points_x.size() -1; i++)
    {
        double X_left = points_x[i] - k;
        double X_right = points_x[i+1] + k;

        if( 
            ((i+1) % 2 == 0) 
            && (p.X >= X_left) 
            && (p.X <= X_right)
        )
        {
            bX = true;
            break;
            };
    };

    bool bY = false;
    for(int i = 0; i < points_y.size() -1; i++)
    {
        double Y_left = points_y[i] - k;
        double Y_right = points_y[i+1] + k;

        if(    
            ((i+1) % 2 == 0) 
            && (p.Y >= Y_left)
            && (p.Y <= Y_right)
        )
        {
            bY = true;
            break;
            };
    };

    return bX && bY;
};

//---------------------------------------------------------------------------

bool area::hasLine(const Sea5kgTriangulationLine &L) const
{
    if( !hasPoint(L.p1) || !hasPoint(L.p2))
        return false;

    return hasPoint(L.getMiddlePoint());
};
//---------------------------------------------------------------------------

bool area::hasIntersections(Sea5kgTriangulationLine l, std::vector<Sea5kgTriangulationPoint> &result)
{
    int nCount = 0;
    for(int i = 0; i < this->count(); i++)
    {
        Sea5kgTriangulationLine line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));

        Sea5kgTriangulationPoint p_res;
        if(l.hasIntersection(line_side, p_res))
        {
            nCount++;
            result.push_back(p_res);
        }
    }
    return (nCount > 0);
};
//---------------------------------------------------------------------------

bool area::hasIntersections(triangulation::area &ar, std::vector<Sea5kgTriangulationPoint> &result)
{
    int nCount = 0;
    for(int ar_i = 0; ar_i < ar.count(); ar_i++)
    {
        Sea5kgTriangulationPoint ar_p1 = ar.getPoint(ar_i);
        Sea5kgTriangulationPoint ar_p2 = ar.getPoint((ar_i+1) % ar.count());

        Sea5kgTriangulationLine ar_line(ar_p1, ar_p2);


        for(int i = 0; i < this->count(); i++)
        {
            Sea5kgTriangulationLine line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));
            Sea5kgTriangulationPoint p_res;
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

/*void area::paint(TImage *img)
{
    TColor nColor = img->Canvas->Pen->Color;
    int nWidth = img->Canvas->Pen->Width;

    img->Canvas->Pen->Color = clBlue;
    img->Canvas->Pen->Width = 2;

    if(count() > 0 )
    {
        Sea5kgTriangulationPoint p = getPoint(0);
        img->Canvas->MoveTo(p.X,p.Y);
        img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
    }

    for(int i = 1; i < count(); i++)
    {
      Sea5kgTriangulationPoint p = getPoint(i);
      img->Canvas->LineTo(p.X,p.Y);
      img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
    };

    if(count() > 0 )
    {
      Sea5kgTriangulationPoint p = getPoint(0);
      img->Canvas->LineTo(p.X,p.Y);
    };

    img->Canvas->Pen->Color = nColor;
    img->Canvas->Pen->Width = nWidth;

};*/

//---------------------------------------------------------------------------

double area::getSquare()
{
    Sea5kgTriangulationPoint pMiddlePoint = getMiddlePoint();
    double square = 0;
    for(int i = 0; i < count(); i++)
    {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        square += Sea5kgTriangulationTriangle(p1, p2, pMiddlePoint).getSquare();
    };
    return square;
};

//---------------------------------------------------------------------------

Sea5kgTriangulationPoint area::getMiddlePoint()
{
    Sea5kgTriangulationPoint pMiddlePoint;
    for(int i = 0; i < count(); i++)
        pMiddlePoint += getPoint(i);
    pMiddlePoint = pMiddlePoint / count();
    return pMiddlePoint;
};

//---------------------------------------------------------------------------

const double area::getPerpendicularToLine(const Sea5kgTriangulationPoint &p, Sea5kgTriangulationPoint &result)
{
    Sea5kgTriangulationPoint res2;
    double nLen2 = 1000;

    for(int i = 0; i < count(); i++)
    {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        Sea5kgTriangulationLine L(p1, p2);
        Sea5kgTriangulationPoint res;
        Sea5kgTriangulationPoint p_buff;
        double nLen = L.getPerpendicularToLine(p, res);
        if( nLen < nLen2 && (L.hasPoint(res) || L.getPerpendicularToLine(res, p_buff) < 1))
        {
            res2 = res;
            nLen2 = nLen;
        };
    };

    result = res2;
    return nLen2;
};

//---------------------------------------------------------------------------

bool area::findNearPointSide(const Sea5kgTriangulationPoint &p, Sea5kgTriangulationPoint &result, double len)
{
    Sea5kgTriangulationPoint res2;
    double nLen2 = len;

    for(int i = 0; i < count(); i++)
    {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        double nLen = p1.calcLength(p);
        if(nLen2 >= nLen)
        {
            res2 = p1;
            nLen2 = p1.calcLength(p);
        };
    };

    if(nLen2 < len)
    {
        result = res2;
        return true;
    };

    nLen2 = len;

    //Sea5kgTriangulationPoint res2;
    //double nLen2 = len;

    for (int i = 0; i < count(); i++) {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        Sea5kgTriangulationLine L(p1, p2);
        Sea5kgTriangulationPoint res;
        Sea5kgTriangulationPoint p_buff;
        double nLen = L.getPerpendicularToLine(p, res);
        double nLen_ = L.getPerpendicularToLine(res, p_buff);
        if( nLen < nLen2 && (L.hasPoint(res) && res.calcLength(p_buff) < 1))
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

bool area::getMinMaxXY(double &minX, double &maxX, double &minY, double &maxY)
{
    double _maxX, _minX, _maxY, _minY;

    bool bFirst = true;

    for(int i1 = 0; i1 < count(); i1++)
    {
        Sea5kgTriangulationPoint p = getPoint(i1);

        if(bFirst)
        {
            _maxX = p.X;
            _minX = p.X;
            _maxY = p.Y;
            _minY = p.Y;
            bFirst = false;
        };

        if(_maxX < p.X) _maxX = p.X;
        if(_minX > p.X) _minX = p.X;
        if(_maxY < p.Y) _maxY = p.Y;
        if(_minY > p.Y) _minY = p.Y;
    };

    if((_maxX > _minX) && (_maxY > _minY))
    {
       maxX = _maxX;
       minX = _minX;
       maxY = _maxY;
       minY = _minY;
       return true;
    };
    return false;
};

//---------------------------------------------------------------------------

}; // namespace triangulation



//---------------------------------------------------------------------------
#pragma package(smart_init)
