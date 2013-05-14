//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_triangulator.h"
//---------------------------------------------------------------------------
namespace triangulation {

triangulator::triangulator(triangulation::logger *pLogger)
:
	m_pLogger(pLogger),
	m_nDivisor(2),
	m_nInfelicity(2.0)
{

};

//---------------------------------------------------------------------------

void triangulator::fillArray(const triangulation::point &p, std::vector<triangulation::point> &p_arr, double r)
{
	triangulation::point p_temp;
	triangulation::point p_buff;

	std::vector<triangulation::point> p_vect;

	p_vect.push_back(p);

	p_temp = p;

	if(findNearPoint(p_temp, p_buff, r)) p_vect.push_back(p_buff);
	else p_vect.push_back(p_temp);
	
	/*p_temp = p_vect[p_vect.size()-1];
	if(findNearPointSide(p_temp, p_buff, r)) p_vect.push_back(p_buff);
	else p_vect.push_back(p_temp);*/

	p_temp = p;
	if(findNearPointSide(p_temp, p_buff, r)) p_vect.push_back(p_buff);
	else p_vect.push_back(p_temp);

	p_temp = p_vect[p_vect.size()-1];
        if(findNearPoint(p_temp, p_buff, r)) p_vect.push_back(p_buff);
	else p_vect.push_back(p_temp);

	p_arr.clear();
	for(unsigned int i = 0; i < p_vect.size(); i++)
	{
		unsigned int id = p_vect.size() - i - 1;
		p_arr.push_back(p_vect[id]);
	};
};

//---------------------------------------------------------------------------


void calcPoints(const triangulation::line &L, triangulation::point &p4, triangulation::point &p5, double m_h)
{
	triangulation::point p3 = L.getMiddlePoint();

 	if(L.p1.X == L.p2.X)
 	{
 		p4.X = L.p1.X + m_h;
 		p4.Y = p3.Y;

 		p5.X = L.p1.X - m_h;
 		p5.Y = p3.Y;
 	}
 	else if(L.p1.Y == L.p2.Y )
 	{
 		p4.Y = L.p1.Y + m_h;
 		p4.X = p3.X;

 		p5.Y = L.p1.Y - m_h;
 		p5.X = p3.X;
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

bool triangulator::addTriangle(
	triangulation::point p1,
	triangulation::point p2,
	triangulation::point p3,
	double a,
	std::vector<triangulation::line> &m_lines
)
{
	double p = (a + a + a) / 2;
	double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
	min_square = min_square / 10;

	

 	triangulation::point p_null;

	std::vector<triangulation::point> p1_arr;
	std::vector<triangulation::point> p2_arr;
	std::vector<triangulation::point> p3_arr;

	fillArray(p1, p1_arr, a / m_nDivisor);
	fillArray(p2, p2_arr, a / m_nDivisor);
	fillArray(p3, p3_arr, a / m_nDivisor);

	// possiable triangles 
	std::vector<triangulation::triangle> triangles;

	for(int i = 0; i < p1_arr.size(); i++ )
	{
		if(p1_arr[i] != p_null && p2_arr[i] != p_null && p3_arr[i] != p_null)
			triangles.push_back(triangulation::triangle(p1_arr[i], p2_arr[i], p3_arr[i]));
	};

	for(unsigned int i = 0; i < triangles.size(); i++)
	{
		triangulation::triangle tr = triangles[i];

		if(addTriangleAsIs(tr.p1,tr.p2,tr.p3, a, m_lines))
			return true;
	};

/*	if(addTriangleAsIs(p1,p2,p3, a, m_lines))
		return true;*/


	// return false;
/*
  double k = 0.1;
	triangulation::point p_buff;	
	if( findNearPoint(p1, p_buff, a/m_nDivisor) && p_buff.length(p2) > k && p_buff.length(p3) > k && triangulation::triangle(p_buff,p2,p3).getSquare() >= min_square )	
		p1 = p_buff;

	if( findNearPoint(p2, p_buff, a/m_nDivisor) && p_buff.length(p1) > k && p_buff.length(p3) > k && triangulation::triangle(p1,p_buff,p3).getSquare() >= min_square )
		p2 = p_buff;
*/	
/*
	bool bDebbug = false;
	if(p1.toString() == "(267,129)" && p2.toString() == "(211,97)")	
	{
		bDebbug = true;
		m_pLogger->info("yes! : " + p3.toString());
	};
 
	if(p1.toString() == "(195,371)" && p2.toString() == "(233,415)")
  		m_pLogger->info(" start here!!!!");

  	{
		triangulation::point p_buff;
		if(m_areas[m_nCurrArea].findNearPointSide(p3, p_buff, a/m_nDivisor))
		{
			if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
				return true;
		};
	}


	{
		triangulation::point p_buff;
		for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
		{
			if(i2 != m_nCurrArea && m_areas[i2].findNearPointSide(p3, p_buff, a/3))
			{
				if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
					return true;
			}
		}
	}


	if(addTriangleAsIs(p1,p2,p3, a, m_lines))
		return true;
*/
	/*
	if(hasCurrentArea(p3))
	{
	  // m_pLogger->info("hasCurrentArea == true p3 = " + p3.toString());

		triangulation::point p_buff;
		bool br = m_areas[m_nCurrArea].findNearPointSide(p3, p_buff, a/2);
		// m_pLogger->info("hasCurrentArea == true try add(p1,p2,p3): " + triangulation::triangle(p1,p2,p3).toString());
		

		if(br && triangulation::triangle(p1,p2,p_buff).getSquare() >= min_square) // && hasCurrentArea(p_buff))
		{
		  // m_pLogger->info("hasCurrentArea == true try add: " + triangulation::triangle(p1,p2,p3).toString());
		  // m_pLogger->info("hasCurrentArea == true try add: " + triangulation::triangle(p1,p2,p_buff).toString());

		  // m_pLogger->info("hasCurrentArea not added");
		  //p3 = p_buff;
		  if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
			return true;
    		};
    		// m_pLogger->info("hasCurrentArea == true try add(p1,p2,p_buff): " + triangulation::triangle(p1,p2,p_buff).toString());

    		if(addTriangleAsIs(p1,p2,p3, a, m_lines))
    			return true;

 	// m_pLogger->info("hasCurrentArea not added");
 	}
	else
	{
		// m_pLogger->info("hasCurrentArea == false p3 = " + p3.toString());
		triangulation::point p_buff;
		for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
		{
			if(m_areas[i2].findNearPointSide(p3, p_buff, a/2))
			{
				if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
					return true;
			}
		};
	};

	if(bDebbug) m_pLogger->info("1) p3 = " + p3.toString());

	if(addTriangleAsIs(p1,p2,p3, a, m_lines))
		return true;	  
 	*/
	
/*	if(addTriangleAsIs(p1,p2,p3, a, m_lines))
		return true;*/
/*
	if(!hasCurrentArea(p3))
	{	
		double k = 0.1;

		for(unsigned int i2 = 0; i2 < m_areas.size(); i2++ )
		{
			std::vector<triangulation::point> result;

			result.clear();
			result.push_back(p3);

			if( m_areas[i2].hasIntersections(triangulation::line(p1,p3), result) )
			{

				triangulation::point p_buff;
				for(int i3 = 0; i3 < result.size(); i3++)
				{
					bool br = m_areas[i2].findNearPointSide(result[i3], p_buff, a/2);
					if(bDebbug) m_pLogger->info("2) p3 = " + p_buff.toString());
					if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
						return true;
				};
			};

			result.clear();
			result.push_back(p3);

			if( m_areas[i2].hasIntersections(triangulation::line(p2,p3), result) )
			{

				triangulation::point p_buff;
				for(int i3 = 0; i3 < result.size(); i3++)
				{
					bool br = m_areas[i2].findNearPointSide(result[i3], p_buff, a/2);

				// if(br && p_buff.length(p1) > k && p_buff.length(p2) > k ) // && hasCurrentArea(p_buff))
					{
						if(bDebbug) m_pLogger->info("p3 = " + p_buff.toString());
						if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
							return true;
					};
				};
			};
		};
	};

*/

	{	
		triangulation::triangle tr;	
		triangulation::triangle tr_(p1,p2,p3);	
		triangulation::point p_intersection;
		if(hasIntersectionWithOtherTriangles(tr_, tr, p_intersection))
		{
			if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p1, m_nInfelicity, m_lines))
				return true;
		
			if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p2, m_nInfelicity, m_lines))
				return true;
		
			if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p3, m_nInfelicity, m_lines))
				return true;

			//p3 = p_intersection;
			//if(addTriangleAsIs(p1,p2,p3, 1, m_lines))
			//	return true;
		};
	};

	for(int i = 0; i < m_areas[m_nCurrArea].count(); i++)
	{
		triangulation::point ar_p1 = m_areas[m_nCurrArea].getPoint(i);
		triangulation::point ar_p2 = m_areas[m_nCurrArea].getPoint((i+1) % m_areas[m_nCurrArea].count());
		triangulation::point ar_p3 = m_areas[m_nCurrArea].getPoint((i+2) % m_areas[m_nCurrArea].count());
						
		triangulation::line L1(ar_p1, ar_p2);
		triangulation::line L2(ar_p2, ar_p3);
		triangulation::point p_buff;
			
		if( (L1.hasPoint(p1) && L2.hasPoint(p2))
		    || (L2.hasPoint(p1) && L1.hasPoint(p2))
		    || (ar_p1.length(p1) < m_nInfelicity || ar_p2.length(p2) < m_nInfelicity)
		    || (ar_p1.length(p2) < m_nInfelicity || ar_p2.length(p1) < m_nInfelicity)
		)
		{			
			if(triangulation::line(p1,ar_p2).length() < a + a/m_nDivisor && triangulation::line(p2,ar_p2).length() < a + a/m_nDivisor)
			{
  				if(addTriangleAsIs(p1,p2,ar_p2, m_nInfelicity, m_lines))
					return true;
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
	double a,
	std::vector<triangulation::line> &m_lines
	)
{
	double p = (a + a + a) / 2;
	double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
	min_square = min_square / 3;

	double r = a/m_nDivisor;
	triangulation::point p_altr;	
	if(findNearPoint(p1, p_altr, r))  p1 = p_altr;
	if(findNearPoint(p2, p_altr, r))  p2 = p_altr;
	if(findNearPoint(p3, p_altr, r))  p3 = p_altr;


//	for(unsigned int i = 0; i < triangles.size(); i++)
//	{
		triangulation::triangle tr(p1,p2,p3);

		if( !hasCurrentArea(tr.p1) || !hasCurrentArea(tr.p2) || !hasCurrentArea(tr.p3) )
			return false;

		if( !hasCurrentArea(tr.p1,tr.p2) || !hasCurrentArea(tr.p2,tr.p3) || !hasCurrentArea(tr.p3,tr.p1) )
			return false;

		triangulation::triangle tr_buff;
		triangulation::point p_intersection;		                                         				

		if(tr.p1.length(tr.p2) < m_nInfelicity || tr.p1.length(tr.p3) < m_nInfelicity || tr.p2.length(tr.p3) < m_nInfelicity )
			return false;

		if(
				!findTriangle(tr.p1,tr.p2,tr.p3) 
				&& !hasIntersectionWithOtherTriangles(tr, tr_buff, p_intersection)
		)
		{		
			if(tr.getSquare() > min_square)		
			{
				m_triangles.push_back(tr);
	  		m_lines.push_back(triangulation::line(tr.p1,tr.p3));
	  		m_lines.push_back(triangulation::line(tr.p2,tr.p3));

  			// m_pLogger->info(triangulation::triangle(p1,p2,p3).toString());
  			//if(tr.p2.toString() == "(195,371)" && tr.p3.toString() == "(233,415)")
  			//   m_pLogger->info(" add line here!!!!");

	  		//m_lines.push_back(triangulation::line(tr.p2,tr.p3));
  			return true;
			};
		}
//  	};
	return false;
};

//---------------------------------------------------------------------------

bool triangulator::hasCurrentArea(triangulation::point p)
{
	for(int i = 0; i < m_areas.size(); i++)
	{
		if( i != m_nCurrArea && m_areas[i].hasPoint(p) )
		{
			triangulation::point p_buff;
			if(m_areas[i].getPerpendicularToLine(p, p_buff) > m_nInfelicity)
				return false;
		};
	}
		
	return m_areas[m_nCurrArea].hasPoint(p);
};

//---------------------------------------------------------------------------

bool triangulator::hasCurrentArea(triangulation::point p1, triangulation::point p2)
{
	triangulation::line L(p1,p2);
	for(int i = 0; i < m_areas.size(); i++)
	{
		if( i != m_nCurrArea && m_areas[i].hasLine(L) )
			return false;
	}
	return m_areas[m_nCurrArea].hasLine(L);
};

//---------------------------------------------------------------------------

bool hasPoint_(triangulation::triangle tr, const triangulation::point &p, triangulation::triangle &tr_result, triangulation::point &p_result, triangulation::triangle tr1)
{
	if( (tr.hasPoint(p) && !tr.hasTop(p)))
	{
		p_result = p;
		tr_result = tr1;
		return true;			
	}	
	return false;
};


bool triangulator::hasIntersectionWithOtherTriangles(const triangulation::triangle &tr_, triangulation::triangle &result, triangulation::point &p_result)
{
	triangulation::line L[3];
	L[0] = triangulation::line(tr_.p1,tr_.p2);
	L[1] = triangulation::line(tr_.p2,tr_.p3);
	L[2] = triangulation::line(tr_.p3,tr_.p1);

	for(int i = 0; i < m_triangles.size(); i++)
	{
		triangulation::triangle tr = m_triangles[i];
		triangulation::line TR_L[3];

/*		if( hasPoint_(tr, p1, result, p_result, tr) )
			return true;	

		if( hasPoint_(tr, p2, result, p_result, tr) )
			return true;	

		if( hasPoint_(tr, p3, result, p_result, tr) )
			return true;	

		if( hasPoint_(tr_, tr.p1, result, p_result, tr) )
			return true;	

		if( hasPoint_(tr_, tr.p2, result, p_result, tr) )
			return true;	

		if( hasPoint_(tr_, tr.p3, result, p_result, tr) )
			return true;	
*/
		TR_L[0] = triangulation::line(m_triangles[i].p1, m_triangles[i].p2);
		TR_L[1] = triangulation::line(m_triangles[i].p2, m_triangles[i].p3);
		TR_L[2] = triangulation::line(m_triangles[i].p3, m_triangles[i].p1);

/*
		if( hasPoint_(tr, L[0].getMiddlePoint(), result, p_result, tr) )
			return true;	

		if( hasPoint_(tr, L[1].getMiddlePoint(), result, p_result, tr) )
			return true;	

		if( hasPoint_(tr, L[2].getMiddlePoint(), result, p_result, tr) )
			return true;	
*/

/*
		if( hasPoint_(tr_, TR_L[0].getMiddlePoint(), result, p_result, tr) )
			return true;	

		if( hasPoint_(tr_, TR_L[1].getMiddlePoint(), result, p_result, tr) )
			return true;	

		if( hasPoint_(tr_, TR_L[2].getMiddlePoint(), result, p_result, tr) )
			return true;	
*/
		triangulation::point p;

		for (int i1 = 0; i1 < 3; i1++)
			for (int i2 = 0; i2 < 3; i2++)
				if( TR_L[i1].hasIntersection(L[i2], p) && TR_L[i1].hasPoint(p) && L[i2].hasPoint(p) && !tr.hasTop(p) && !tr_.hasTop(p))
				{
					p_result = p;
					result = tr;
					return true;
				};

	  /*if(tr.hasPoint(p1) || tr.hasPoint(p2) || tr.hasPoint(p3))
	  {
			result = tr;
			p_result = triangulation::point();
			return true;	  
	  };*/

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

void triangulator::findPoints(
	const triangulation::point &p1, 
	const std::vector<triangulation::point> &points, 
	double a, 
	std::vector<triangulation::triangle> &triangles
)
{
	double k = 2;
	double a_min2 = k;
	double a_min1 = a - k;
	double a_max = a + a/m_nDivisor + k;
	for(unsigned int iP2 = 0; iP2 < points.size(); iP2++)
	for(unsigned int iP3 = 0; iP3 < points.size(); iP3++)
	{
		if(iP2 == iP3) continue;

		triangulation::point p2 = points[iP2];
		triangulation::point p3 = points[iP3];
		
		double nLen1 = p1.length(p2);
		double nLen2 = p2.length(p3);
		double nLen3 = p3.length(p1);

		
		if(
			nLen1 > a_min2 &&  nLen2 > a_min2 && nLen3 > a_min2 &&
			nLen1 <= a_max && nLen2 <= a_max && nLen3 <= a_max
		)
		{
			triangles.push_back(triangulation::triangle(p1,p2,p3));
		};
	
		//if(nLen1 <= (a + k))
		//	triangles.push_back(triangulation::triangle(p1,p2,p3));
	};
/*	
	for(unsigned int iP2 = 0; iP2 < points.size(); iP2++)
	for(unsigned int iP3 = 0; iP3 < points.size(); iP3++)
	{
		if(iP2 == iP3) continue;

		triangulation::point p2 = points[iP2];
		triangulation::point p3 = points[iP3];
		
		double nLen1 = p1.length(p2);
		double nLen2 = p2.length(p3);
		double nLen3 = p3.length(p1);

		
		if(
			nLen1 > a_min2 &&  nLen2 > a_min2 && nLen3 > a_min2 &&
			nLen1 <= a_min1 && nLen2 <= a_min1 && nLen3 <= a_min1
		)
		{
			triangles.push_back(triangulation::triangle(p1,p2,p3));
		};
	
		//if(nLen1 <= (a + k))
		//	triangles.push_back(triangulation::triangle(p1,p2,p3));
	};
*/
	if( triangles.size() == 0 && points.size() > 0)
	{
		findPoints(p1, points, a + a/m_nDivisor, triangles);
	};
};

//---------------------------------------------------------------------------

void triangulator::triangulate_add_triangles()
{

};

//---------------------------------------------------------------------------

void triangulator::triangulate_resizing()
{

};

//---------------------------------------------------------------------------

void triangulator::triangulate_remove_triangles()
{

};

//---------------------------------------------------------------------------

void triangulator::step_first()
{
	for(int i = 0; i < m_areas.size(); i++)
	{
		m_nCurrArea = i;

		int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
		double square = m_areas[m_nCurrArea].getSquare();
		double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
		double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
		double h = 2 * approximateTriangle / a;
		double r = a * a * a / sqrt((3*a) * a * a * a );

		m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
		m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
		m_pLogger->info("a = " + FloatToStr(a) + " px");
		m_pLogger->info("h = " + FloatToStr(h) + " px");
		m_pLogger->info("r = " + FloatToStr(r) + " px");

		
		std::vector<triangulation::point> points;
		double nStartX = 0;
 		triangulation::point p_before;
		for(double iY = 0; iY < 1024; iY = iY + h)
		{
			if( nStartX < 0 )
			  nStartX = 0;
			else
			  nStartX = (-a)/2;

			p_before = triangulation::point();

			for(double iX = nStartX; iX < 1024; iX = iX + a)
			{
				triangulation::point p(iX, iY);
				triangulation::point p_buff;


				if(hasCurrentArea(p))
				{
					//points.push_back(p);

					if(findNearPointSide(p, p_buff, a/m_nDivisor))
						points.push_back(p_buff);
					else
						points.push_back(p);
				}
				else if(findNearPointSide(p, p_buff, a/m_nDivisor))
				{            	
					//m_pLogger->info("added p = " + p5.toString());
					points.push_back(p_buff);	
				}


/*
				triangulation::line same_line[5];
				triangulation::point p4, p5;
				same_line[0] = triangulation::line(p, p_before);
                                calcPoints(same_line[0], p4, p5, h);
                                same_line[1] = triangulation::line(p, p4);
                                same_line[2] = triangulation::line(p_before, p4);
                                same_line[3] = triangulation::line(p, p5);
                                same_line[4] = triangulation::line(p_before, p5);
                                

				for(int iL = 0; iL < 5; iL++)
				{
					std::vector<triangulation::point> points_intersection;
					if( m_areas[m_nCurrArea].hasIntersections(same_line[iL], points_intersection) )
					{
						m_pLogger->info("p4 = " + p4.toString());
						m_pLogger->info("p5 = " + p5.toString());
						points.push_back(points_intersection[0]);
					};
				};
*/
/*				for(unsigned int iPs = 0; iPs < points_intersection.size(); iPs++)
				{
					points.push_back(points_intersection[iPs]);

//					if(hasCurrentArea(p_buff))
					//points.push_back(p_buff);
					// points.push_back(p_buff);
				};*/
				p_before = p;
			};
		};
		
		for(unsigned int iP = 0; iP < points.size(); iP++)
		{
			std::vector<triangulation::triangle> triangles;
			std::vector<triangulation::line> lines;
			triangulation::point p1 = points[iP];
			findPoints(p1, points, a, triangles);
			int sch = 0;
			for(unsigned int iT = 0; iT < triangles.size(); iT++)
			{
				triangulation::triangle tr = triangles[iT];
				triangulation::triangle tr_;
				triangulation::point p_intersection;

				if(addTriangleAsIs(tr.p1,tr.p2,tr.p3, a, lines))
				{
					sch++;
				}

                               /*if(!findTriangle(tr.p1,tr.p2,tr.p3) && !hasIntersectionWithOtherTriangles(tr, tr_, p_intersection))
				   m_triangles.push_back(tr);*/
			};
//			if(triangles.size() != 0 && triangles.size() - sch == )
		};
	};
};

class Triangles_sp
{
	AddPoint();

};


bool findNearPoint_sp(std::vector<triangulation::point *> &pPoints, triangulation::point *p, triangulation::point **pResult, double a)
{
	triangulation::point pP(p->X, p->Y);
	double curr_len = -1;	
	int iFound = -1;
	for(unsigned int i = 0; i < pPoints.size(); i++)
	{
		double len = pPoints[i]->length(pP);

		if(len < a)
		{
			if(iFound == -1)
			{
				curr_len = len;
				iFound = i; 
			}
			else if(curr_len > len)
			{
				curr_len = len;
				iFound = i;
			}
		}    
	};

	if(iFound == -1) return false;

	*pResult = pPoints[iFound];
	return true;
};

void triangulator::step_first_alien()
{
	for(int i = 0; i < m_areas.size(); i++)
	{
		m_nCurrArea = i;

		int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
		double square = m_areas[m_nCurrArea].getSquare();
		double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
		double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
		double h = 2 * approximateTriangle / a;
		double r = a * a * a / sqrt((3*a) * a * a * a );

		m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
		m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
		m_pLogger->info("a = " + FloatToStr(a) + " px");
		m_pLogger->info("h = " + FloatToStr(h) + " px");
		m_pLogger->info("r = " + FloatToStr(r) + " px");
		
		std::vector<triangulation::point *> pPoints;
		std::vector<triangulation::triangle_pointer *> pTriangules;
		double nStartX = 0, nEndX = 0;
		double minX,maxX,minY,maxY;

		m_areas[m_nCurrArea].getMinMaxXY(minX, maxX, minY, maxY);

		m_pLogger->info("minX = " + FloatToStr(minX) + " px");
		m_pLogger->info("maxX = " + FloatToStr(maxX) + " px");
		m_pLogger->info("minY = " + FloatToStr(minY) + " px");
		m_pLogger->info("maxY = " + FloatToStr(maxY) + " px");

		double a2 = a/2;
		double a4 = a/4;
		double a10 = a/10;
		minX -= a;
		maxX += a;

		int iS = 0;
		for(double iY = minY - h; iY < maxY + h; iY = iY + h)
		{
			iS = iS + 1;
			if( iS % 2 == 0)
			{		
				nStartX = minX;
				nEndX = maxX;
			}
			else
			{
				nStartX = minX - a2;
				nEndX = maxX + a2;
			}

			for(double iX = nStartX; iX < nEndX; iX = iX + a)
			{
  				triangulation::point p1_(iX, iY);
				triangulation::point p2_(iX + a, iY);
				triangulation::point p3_;
				triangulation::point p4_;
				triangulation::line li_(p1_,p2_);

				triangulation::point *p1 = new triangulation::point(p1_.X, p1_.Y);
				triangulation::point *p2 = new triangulation::point(p2_.X, p2_.Y);
				
				calcPoints(li_,p3_,p4_,h);
				
				triangulation::point *p3 = new triangulation::point(p3_.X, p3_.Y);
				triangulation::point *p4 = new triangulation::point(p4_.X, p4_.Y);

				triangulation::point *pFound;

				/*
				pPoints.push_back(p1);
				pPoints.push_back(p2);
				pPoints.push_back(p3);
				pPoints.push_back(p4);
				*/
				m_pLogger->info(" find Near Point ");

				if( findNearPoint_sp(pPoints, p1, &pFound, a4))
					*p1 = *pFound;
				else
					pPoints.push_back(p1);

				if( findNearPoint_sp(pPoints, p2, &pFound, a4))
					*p2 = *pFound;
				else
					pPoints.push_back(p2);

				if( findNearPoint_sp(pPoints, p3, &pFound, a4))
					*p3 = *pFound;
				else
					pPoints.push_back(p3);

				if( findNearPoint_sp(pPoints, p4, &pFound, a4))
					*p4 = *pFound;
				else
					pPoints.push_back(p4);

				m_pLogger->info(" create triangle ");

				triangulation::triangle_pointer *pTrinagle1 = new triangulation::triangle_pointer();
				*(pTrinagle1->p1) = *p1;
				*(pTrinagle1->p2) = *p2;
				*(pTrinagle1->p3) = *p3;

				triangulation::triangle_pointer *pTrinagle2 = new triangulation::triangle_pointer();
				*(pTrinagle2->p1) = *p1;
				*(pTrinagle2->p2) = *p2;
				*(pTrinagle2->p3) = *p4;

				m_pLogger->info(" push ");

				pTriangules.push_back(pTrinagle1);
				pTriangules.push_back(pTrinagle2);					
			}
		}

		m_pLogger->info(" remove triangle which not inside in current area ");


		// remove triangle which has not inside in current area
		std::vector<triangulation::triangle_pointer *> pTriangules_copy = pTriangules;
		pTriangules.clear();
		for(int iT = 0; iT < pTriangules_copy.size(); iT++)
		{
			triangulation::triangle tr = pTriangules_copy[iT]->getTriangle();

			if(hasCurrentArea(tr.p1) || hasCurrentArea(tr.p2) || hasCurrentArea(tr.p3))
			    pTriangules.push_back(pTriangules_copy[iT]);
		}


		m_pLogger->info(" find point near with area of side ");

		// find point near with area of side 
		for(int iT = 0; iT < pTriangules.size(); iT++)
		{
			triangulation::triangle_pointer *tr = pTriangules[iT];
			triangulation::point p1(tr->p1->X, tr->p1->Y);
			triangulation::point p2(tr->p2->X, tr->p2->Y);
			triangulation::point p3(tr->p3->X, tr->p3->Y);

			triangulation::point p_buff;

			if(findNearPointSide(p1, p_buff, a2))
			{
				tr->p1->X = p_buff.X;
				tr->p1->Y = p_buff.Y;
			}

			if(findNearPointSide(p2, p_buff, a2))
			{
				tr->p2->X = p_buff.X;
				tr->p2->Y = p_buff.Y;
			}

			if(findNearPointSide(p3, p_buff, a2))
			{
				tr->p3->X = p_buff.X;
				tr->p3->Y = p_buff.Y;
			}
		};
		
		// remove triangles which has point in inside current area
		pTriangules_copy.clear();
		pTriangules_copy = pTriangules;
		pTriangules.clear();
		for(int iT = 0; iT < pTriangules_copy.size(); iT++)
		{
			triangulation::triangle tr = pTriangules_copy[iT]->getTriangle();
			triangulation::point p12 = triangulation::line(tr.p1, tr.p2).getMiddlePoint();
			triangulation::point p23 = triangulation::line(tr.p2, tr.p3).getMiddlePoint();
			triangulation::point p31 = triangulation::line(tr.p3, tr.p1).getMiddlePoint();

			if( hasCurrentArea(tr.p1) && 
				hasCurrentArea(tr.p2) && 
				hasCurrentArea(tr.p3)
			)
			{
				if( !hasCurrentArea(p12) )
				{
					triangulation::triangle_pointer *tr_ = 	new triangulation::triangle_pointer();

					triangulation::point *new_p = new triangulation::point();
					triangulation::point p_buff;

					if(findNearPointSide(p12, p_buff, a2))
						p12 = p_buff;

					//triangulation::point p1_12 = triangulation::line(tr.p1, p1_12).getMiddlePoint();
					//triangulation::point p2_12 = triangulation::line(tr.p2, p2_12).getMiddlePoint();

					if( hasCurrentArea(p12) ) // && hasCurrentArea(p1_12) && hasCurrentArea(p2_12))
					{
						new_p->X = p12.X;
						new_p->Y = p12.Y;

						tr_->p1 = new_p;
						*(tr_->p2) = *(pTriangules_copy[iT]->p2);
						*(tr_->p3) = *(pTriangules_copy[iT]->p3);
										
						*(pTriangules_copy[iT]->p2) = *(tr_->p1);

						pTriangules.push_back(tr_);
					}
				}					

				if( !hasCurrentArea(p23) )
				{
					triangulation::triangle_pointer *tr_ = 	new triangulation::triangle_pointer();

					triangulation::point *new_p = new triangulation::point();
					triangulation::point p_buff;

					if(findNearPointSide(p23, p_buff, a2))
						p23 = p_buff;
					
					// triangulation::point p2_23 = triangulation::line(tr.p2, p2_23).getMiddlePoint();
					// triangulation::point p3_23 = triangulation::line(tr.p3, p3_23).getMiddlePoint();

					if( hasCurrentArea(p23) ) // && hasCurrentArea(p2_23) && hasCurrentArea(p3_23))
					{

						new_p->X = p23.X;
						new_p->Y = p23.Y;

						*(tr_->p1) = *(pTriangules_copy[iT]->p1);
						tr_->p2 = new_p;
						*(tr_->p3) = *(pTriangules_copy[iT]->p3);
									
						*(pTriangules_copy[iT]->p3) = *(tr_->p2);

						pTriangules.push_back(tr_);
					}
				}					

				if( !hasCurrentArea(p31) )
				{
					triangulation::triangle_pointer *tr_ = 	new triangulation::triangle_pointer();

					triangulation::point *new_p = new triangulation::point();
					triangulation::point p_buff;

					if(findNearPointSide(p31, p_buff, a2))
						p31 = p_buff;

					// triangulation::point p3_31 = triangulation::line(tr.p3, p3_31).getMiddlePoint();
					// triangulation::point p1_31 = triangulation::line(tr.p1, p1_31).getMiddlePoint();
											
					if( hasCurrentArea(p31) ) // && hasCurrentArea(p3_31) && hasCurrentArea(p1_31))
					{
						new_p->X = p31.X;
						new_p->Y = p31.Y;

						*(tr_->p1) = *(pTriangules_copy[iT]->p1);
						*(tr_->p2) = *(pTriangules_copy[iT]->p2);
						tr_->p3 = new_p;
											
						*(pTriangules_copy[iT]->p1) = *(tr_->p3);

						pTriangules.push_back(tr_);	
					}
				}					



				pTriangules.push_back(pTriangules_copy[iT]);
				
			}
		    
		}
		
		// remove triangles which as line		
		pTriangules_copy.clear();
		pTriangules_copy = pTriangules;
		pTriangules.clear();
		for(int iT = 0; iT < pTriangules_copy.size(); iT++)
		{
			triangulation::triangle tr = pTriangules_copy[iT]->getTriangle();
			triangulation::line L12(tr.p1, tr.p2);
			triangulation::line L23(tr.p2, tr.p3);
			triangulation::line L31(tr.p3, tr.p1);
			triangulation::point p_temp;

			if( L12.getPerpendicularToLine(tr.p3, p_temp) > a10 && 
				L23.getPerpendicularToLine(tr.p1, p_temp) > a10 && 
				L31.getPerpendicularToLine(tr.p2, p_temp) > a10)
				pTriangules.push_back(pTriangules_copy[iT]);						    
		}
		

		m_pLogger->info(" print ");
		
		for(int iT = 0; iT < pTriangules.size(); iT++)
		{
			triangulation::triangle_pointer *tr = pTriangules[iT];
			triangulation::point p1(tr->p1->X, tr->p1->Y);
			triangulation::point p2(tr->p2->X, tr->p2->Y);
			triangulation::point p3(tr->p3->X, tr->p3->Y);

			triangulation::triangle tr1(p1,p2,p3);
			m_triangles.push_back(tr1);			
		}
	};
};


void triangulator::triangulate()
{
	clear_triangles();

//	step_first();
	step_first_alien();
/*
	for(int i = 0; i < m_areas.size(); i++)
	{
		m_nCurrArea = i;

		int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
		double square = m_areas[m_nCurrArea].getSquare();
		double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
		double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
		double h = 2 * approximateTriangle / a;
		double r = a * a * a / sqrt((3*a) * a * a * a );

		m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
		m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
		m_pLogger->info("a = " + FloatToStr(a) + " px");
		m_pLogger->info("h = " + FloatToStr(h) + " px");
		m_pLogger->info("r = " + FloatToStr(r) + " px");

		for(int i1 = 0; i1 < m_areas[m_nCurrArea].count(); i1++ )
		{					
			int i2 = (i1+1) % m_areas[m_nCurrArea].count();
			triangulation::point p1 = m_areas[m_nCurrArea].getPoint(i1);
			triangulation::point p2 = triangulation::getNextPoint(p1, m_areas[m_nCurrArea].getPoint(i2), a);
			int nSize = m_triangles.size();
			netting nett(p1, p2, h, a, this);
			nett.processing();
			if(m_triangles.size() != nSize)
			   break;	
		}
	};
*/
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
		if(m_triangles[i].isEqual(tr))
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
				curr_len = triangulation::line(p, res).length();
			};
		};
	};

	if(curr_len == -1) return false;

	result = res;
	return true;
};

//---------------------------------------------------------------------------

bool triangulator::findNearPointSide(triangulation::point p, triangulation::point &result, double r)
{
  	{
		triangulation::point p_buff;
		if(m_areas[m_nCurrArea].findNearPointSide(p, p_buff, r))
		{
			result = p_buff;
			return true;
		};
	}

	{
		triangulation::point p_buff;
		for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
		{
			if(i2 != m_nCurrArea && m_areas[i2].findNearPointSide(p, p_buff, r))
			{
				result = p_buff;
				return true;
			}
		}
	}
	return false;
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
  // clever add triangle as is
	while(m_lines.size() > 0)
	{
		triangulation::line L = m_lines[0];
		m_lines.erase(m_lines.begin());

		triangulation::point p4, p5;
		calcPoints(L, p4, p5);


  	bool bAdd4 = m_tr->addTriangleAsIs(L.p1,L.p2,p4, m_a, m_lines);
  	bool bAdd5 = m_tr->addTriangleAsIs(L.p1,L.p2,p5, m_a, m_lines);	  	
	};

	/*
	std::vector<triangulation::triangle> triangles = m_tr->getTriangles();

	
	// clever add triangle

	for(unsigned int i = 0; i < triangles.size(); i++)
	{
		triangulation::triangle tr = triangles[i];
		m_lines.push_back(triangulation::line(tr.p1,tr.p2));
		m_lines.push_back(triangulation::line(tr.p2,tr.p3));
		m_lines.push_back(triangulation::line(tr.p3,tr.p1));
	};

	while(m_lines.size() > 0)
	{
		triangulation::line L = m_lines[0];
		m_lines.erase(m_lines.begin());

		triangulation::point p4, p5;
		calcPoints(L, p4, p5);

  	bool bAdd4 = m_tr->addTriangle(L.p1,L.p2,p4, m_a, m_lines);
  	bool bAdd5 = m_tr->addTriangle(L.p1,L.p2,p5, m_a, m_lines);	  	
	};
	*/
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
