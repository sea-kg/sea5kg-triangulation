//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_triangulator.h"
//---------------------------------------------------------------------------
namespace triangulation {

triangulator::triangulator(triangulation::logger *pLogger)
:
	m_pLogger(pLogger)
{

};

//---------------------------------------------------------------------------

bool triangulator::addTriangle(
	triangulation::point p1,
	triangulation::point p2,
	triangulation::point p3,
	double a,
	std::vector<triangulation::line> &m_lines
)
{
	bool bDebbug = false;
	if(p1.toString() == "(267,129)" && p2.toString() == "(211,97)")	
	{
		bDebbug = true;
		m_pLogger->info("yes! : " + p3.toString());
	};

	bool bShouldChange_p3;
  
	double p = (a + a + a) / 2;
	double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
	min_square = min_square / 10;

	if(hasCurrentArea(p3))
	{
		triangulation::point p_buff;
		bool br = m_areas[nCurrArea].findNearPointSide(p3, p_buff, a/2);
		
		if(br && triangulation::triangle(p1,p2,p_buff).getSquare() >= min_square) // && hasCurrentArea(p_buff))
		{
		  p3 = p_buff;			
		};
	};

	if(bDebbug) m_pLogger->info("1) p3 = " + p3.toString());
	if(addTriangleAsIs(p1,p2,p3, bShouldChange_p3, a, m_lines))
		return true;	  


/*	if(addTriangleAsIs(p1,p2,p3, bShouldChange_p3, a, m_lines))
		return true;*/

	// ShowMessage("bShouldChange_p3_2");

	if(!hasCurrentArea(p3))
	{	
		double k = 0.1;
		
		std::vector<triangulation::point> result;
		result.push_back(p3);

		if( m_areas[nCurrArea].hasIntersections(triangulation::line(p1,p3), result) )
		{			
		  
			triangulation::point p_buff;
			for(int i3 = 0; i3 < result.size(); i3++)
			{
				bool br = m_areas[nCurrArea].findNearPointSide(result[i3], p_buff, a/2);	
				if(bDebbug) m_pLogger->info("2) p3 = " + p_buff.toString());
				if(addTriangleAsIs(p1,p2,p_buff, bShouldChange_p3, a, m_lines))
					return true;
			};			
		};
				
		result.clear();
		result.push_back(p3);

		if( m_areas[nCurrArea].hasIntersections(triangulation::line(p2,p3), result) )
		{
			
			triangulation::point p_buff;
			for(int i3 = 0; i3 < result.size(); i3++)
			{
  				bool br = m_areas[nCurrArea].findNearPointSide(result[i3], p_buff, a/2);	
  			
  			// if(br && p_buff.length(p1) > k && p_buff.length(p2) > k ) // && hasCurrentArea(p_buff))
  				{  	
					if(bDebbug) m_pLogger->info("p3 = " + p_buff.toString());
  					if(addTriangleAsIs(p1,p2,p_buff, bShouldChange_p3, a, m_lines))
  						return true;
	  			};
  			};
		};
	};

			
	triangulation::triangle tr;	
	triangulation::point p_intersection;
	if(hasIntersection(p1,p2,p3, tr, p_intersection))
	{
		if(bDebbug) m_pLogger->info("3) p3 = " + tr.p1.toString());
		if(addTriangleAsIs(p1,p2,tr.p1, bShouldChange_p3, 1, m_lines))
			return true;
		
		if(bDebbug) m_pLogger->info("4) p3 = " + tr.p2.toString());
		if(addTriangleAsIs(p1,p2,tr.p2, bShouldChange_p3, 1, m_lines))
			return true;
		
		if(bDebbug) m_pLogger->info("5) p3 = " + tr.p3.toString());
		if(addTriangleAsIs(p1,p2,tr.p3, bShouldChange_p3, 1, m_lines))
			return true;

/*		p3 = p_intersection;
		if(addTriangleAsIs(p1,p2,p3, bShouldChange_p3, 1, m_lines))
			return true;*/
	};

	for(int i = 0; i < m_areas[nCurrArea].count(); i++)
	{
		triangulation::point ar_p1 = m_areas[nCurrArea].getPoint(i);
		triangulation::point ar_p2 = m_areas[nCurrArea].getPoint((i+1) % m_areas[nCurrArea].count());
		triangulation::point ar_p3 = m_areas[nCurrArea].getPoint((i+2) % m_areas[nCurrArea].count());
						
		triangulation::line L1(ar_p1, ar_p2);
		triangulation::line L2(ar_p2, ar_p3);
		triangulation::point p_buff;
			
		if( (L1.hasPoint(p1) && L2.hasPoint(p2))
		    || (L2.hasPoint(p1) && L1.hasPoint(p2))
		    || (ar_p1.length(p1) < 1 || ar_p2.length(p2) < 1)
		    || (ar_p1.length(p2) < 1 || ar_p2.length(p1) < 1)
		)
		{			
		  if(triangulation::line(p1,ar_p2).length() < a + a/2 && triangulation::line(p2,ar_p2).length() < a + a/2)
		  {
			if(bDebbug) m_pLogger->info("6) p3 = " + ar_p2.toString());
  			if(addTriangleAsIs(p1,p2,ar_p2, bShouldChange_p3, 1, m_lines))
  			{
				p3 = ar_p2;
  				return true;
  			};
  		};
		};
	};

	
	return false;
};
//---------------------------------------------------------------------------

bool triangulator::addTriangleAsIs(
	triangulation::point p1, 
	triangulation::point p2, 
	triangulation::point p3, 
	bool &bShouldChange_p3, 
	double a,
	std::vector<triangulation::line> &m_lines
	)
{
	double k = 0.1;
	triangulation::point p_buff;
	if( findNearPoint(p1, p_buff, a/2) && p_buff.length(p2) > k && p_buff.length(p3) > k)
		p1 = p_buff;

	if( findNearPoint(p2, p_buff, a/2) && p_buff.length(p1) > k && p_buff.length(p3) > k)
		p2 = p_buff;
		
	if( findNearPoint(p3, p_buff, a/2) && p_buff.length(p2) > k && p_buff.length(p1) > k)
		p3 = p_buff;
		
	bShouldChange_p3 = false;
	
	if(	!hasCurrentArea(p1) || !hasCurrentArea(p2) || !hasCurrentArea(p3) )
	{
		bShouldChange_p3 = hasCurrentArea(p3);
		return false;
	};

	triangulation::triangle tr;
	triangulation::point p_intersection;

	if(!findTriangle(p1,p2,p3) && !hasIntersection(p1,p2,p3, tr, p_intersection))
	{		
		double p = (a + a + a) / 2;
		double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
		min_square = min_square / 10;

		if(triangulation::triangle(p1,p2,p3).getSquare() > min_square)		
		{
			m_triangles.push_back(triangulation::triangle(p1,p2,p3));
			// m_pLogger->info(triangulation::triangle(p1,p2,p3).toString());
			m_lines.push_back(triangulation::line(p1,p3));
			m_lines.push_back(triangulation::line(p2,p3));
			return true;
    };
	}
	return false;
};

//---------------------------------------------------------------------------

bool triangulator::hasCurrentArea(triangulation::point p)
{
	for(int i = 0; i < m_areas.size(); i++)
	{
		if( i != nCurrArea && m_areas[i].hasPoint(p) )
			return false;
	}
		
	return m_areas[nCurrArea].hasPoint(p);
};
			
//---------------------------------------------------------------------------

bool triangulator::hasIntersection(const triangulation::point &p1, const triangulation::point &p2, const triangulation::point &p3, triangulation::triangle &result, triangulation::point &p_result)
{
	triangulation::line L[3];

	L[0] = triangulation::line(p1,p2);
	L[1] = triangulation::line(p2,p3);
	L[2] = triangulation::line(p3,p1);

	for(int i = 0; i < m_triangles.size(); i++)
	{
		triangulation::triangle tr = m_triangles[i];
		triangulation::line TR_L[3];

		TR_L[0] = triangulation::line(m_triangles[i].p1, m_triangles[i].p2);
		TR_L[1] = triangulation::line(m_triangles[i].p2, m_triangles[i].p3);
		TR_L[2] = triangulation::line(m_triangles[i].p3, m_triangles[i].p1);

		triangulation::point p;

		for (int i1 = 0; i1 < 3; i1++)
			for (int i2 = 0; i2 < 3; i2++)
				if( TR_L[i1].hasIntersection(L[i2], p) && !tr.hasTop(p) )
				{
				  p_result = p;
					result = tr;
					return true;
				};

	  if(tr.hasPoint(p1) || tr.hasPoint(p2) || tr.hasPoint(p3))
	  {
			result = tr;
			p_result = triangulation::point();
			return true;	  
	  };

	};
	return false;
};
//---------------------------------------------------------------------------

void triangulator::addArea(triangulation::area ar)
{
	std::vector<triangulation::point> result;
	
	for(int i = 0; i < m_areas.size(); i++)
	{
		if(m_areas[i].hasIntersections(ar, result))
		{
			ShowMessage("areas has intersection");
			return;
		};
	};
		
	m_areas.push_back(ar);
};
//---------------------------------------------------------------------------

std::vector<triangulation::area> &triangulator::getAreas()
{
	return m_areas;
};
//---------------------------------------------------------------------------

std::vector<triangulation::triangle> &triangulator::getTriangles()
{
	return m_triangles;
};
//---------------------------------------------------------------------------

void triangulator::triangulate()
{
	clear_triangles();

	for(int i = 0; i < m_areas.size(); i++)
	{
		nCurrArea = i;

		int nCountTriangles = m_areas[nCurrArea].getCountTriangles();
		double square = m_areas[nCurrArea].getSquare();
		double approximateTriangle = square / m_areas[nCurrArea].getCountTriangles();
		double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
		double h = 2 * approximateTriangle / a;
		double r = a * a * a / sqrt((3*a) * a * a * a );

		m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
		m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
		m_pLogger->info("a = " + FloatToStr(a) + " px");
		m_pLogger->info("h = " + FloatToStr(h) + " px");
		m_pLogger->info("r = " + FloatToStr(r) + " px");

		for(int i1 = 0; i1 < m_areas[nCurrArea].count(); i1++ )
		{					
			int i2 = (i1+1) % m_areas[nCurrArea].count();
			triangulation::point p1 = m_areas[nCurrArea].getPoint(i1);
			triangulation::point p2 = triangulation::getNextPoint(p1, m_areas[nCurrArea].getPoint(i2), a);
			int nSize = m_triangles.size();
			netting nett(p1, p2, h, a, this);
			nett.processing();
			if(m_triangles.size() != nSize)
			   break;	
		}
	};
};
//---------------------------------------------------------------------------

void triangulator::clear_triangles()
{
    m_triangles.clear();
};
//---------------------------------------------------------------------------

void triangulator::clear_areas()
{
	m_areas.clear();
}
//---------------------------------------------------------------------------

bool triangulator::findTriangle(triangulation::point p1, triangulation::point p2, triangulation::point p3)
{
	triangulation::triangle tr(p1,p2,p3);
	for(int i = 0; i < m_triangles.size(); i++)
	{
		if(m_triangles[i] == tr)
			return true;
	};
	return false;
};

//---------------------------------------------------------------------------

bool triangulator::findNearPoint(triangulation::point p, triangulation::point &result, double r)
{
	triangulation::point res;
	double curr_len = -1;
	for(int i = 0; i < m_triangles.size(); i++)
	{
		triangulation::point p_buff;
		if(m_triangles[i].findNearPoint(p, p_buff, r))
		{
			if(curr_len == -1)
			{
				res = p_buff;
				curr_len = triangulation::line(p, res).length();
			}
			else if(triangulation::line(p, p_buff).length() < curr_len)
			{
				res = p_buff;
			};
		};
	};

	if(curr_len == -1) return false;

	result = res;
	return true;
};

//---------------------------------------------------------------------------

netting::netting(const triangulation::point &p1, const triangulation::point &p2, double h, double a, triangulation::triangulator *tr)
:
	m_h(h), m_a(a), m_tr(tr)
{
	triangulation::line l1(p1,p2);
	if( l1.length() < 1) return;
  m_lines.push_back(l1);
}

//---------------------------------------------------------------------------

void netting::calcPoints(const triangulation::line &L, triangulation::point &p4, triangulation::point &p5)
{
	triangulation::point p3 = L.getMiddlePoint();

 	if(L.p1.X == L.p2.X)
 	{
 		p4.X = L.p1.X;
 		p4.Y = p3.Y + m_h;

 		p5.X = L.p1.X;
 		p5.Y = p3.Y - m_h;
 	}
 	else if(L.p1.Y == L.p2.Y )
 	{
 		p4.Y = L.p1.Y;
 		p4.X = p3.X + m_h;

 		p5.Y = L.p1.Y;
 		p5.X = p3.X - m_h;
 	}
 	else
 	{
 		double k1,k2;
 		k1 = m_h / L.length();
 		k2 = -k1;
 		p4.X = p3.X + (L.p1.Y - L.p2.Y) * k1;
 		p4.Y = p3.Y - (L.p1.X - L.p2.X) * k1;
 		p5.X = p3.X + (L.p1.Y - L.p2.Y) * k2;
 		p5.Y = p3.Y - (L.p1.X - L.p2.X) * k2;
 	};

};

//---------------------------------------------------------------------------

void netting::processing()
{
	while(m_lines.size() > 0)
	{
		triangulation::line L = m_lines[0];
		m_lines.erase(m_lines.begin());

		triangulation::point p4, p5;
		calcPoints(L, p4, p5);

  	bool bAdd4 = m_tr->addTriangle(L.p1,L.p2,p4, m_a, m_lines);
  	bool bAdd5 = m_tr->addTriangle(L.p1,L.p2,p5, m_a, m_lines);	
  	
  	
	};
};

/*
bool recours_netting(triangulation::point &p1, triangulation::point &p2, double h, double a, triangulation::triangulator *tr)
{
	triangulation::line l1(p1,p2);
	if( l1.length() < 1) return false;

	triangulation::point p3 = l1.getMiddlePoint();
	triangulation::point p4, p5;

	if(p1.X == p2.X)
	{
		p4.X = p1.X;
		p4.Y = p3.Y + h;

		p5.X = p1.X;
		p5.Y = p3.Y - h;
	}
	else if(p1.Y == p2.Y )
	{
		p4.Y = p1.Y;
		p4.X = p3.X + h;

		p5.Y = p1.Y;
		p5.X = p3.X - h;
	}
	else
	{
		double k1,k2;
		k1 = h / l1.length();
		k2 = -k1;
		p4.X = p3.X + (p1.Y - p2.Y) * k1;
		p4.Y = p3.Y - (p1.X - p2.X) * k1;
		p5.X = p3.X + (p1.Y - p2.Y) * k2;
		p5.Y = p3.Y - (p1.X - p2.X) * k2;
	};
//	m_pLogger->info();


	bool bAdd4 = tr->addTriangle(p1,p2,p4, a);
	bool bAdd5 = tr->addTriangle(p1,p2,p5, a);
		
	if(bAdd4) recours_netting(p1, p4, h, a, tr);
	if(bAdd5) recours_netting(p1, p5, h, a, tr);	
    if(bAdd4) recours_netting(p2, p4, h, a, tr);	
	if(bAdd5) recours_netting(p2, p5, h, a, tr);

	return false;
};
*/
};
#pragma package(smart_init)
