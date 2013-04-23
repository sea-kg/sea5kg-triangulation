//---------------------------------------------------------------------------

#ifndef _tr_triangulatorH
#define _tr_triangulatorH
#include "_tr_logger.h"
#include "_tr_point.h"
#include "_tr_triangle.h"
#include "_tr_area.h"
#include <vector>

//---------------------------------------------------------------------------
namespace triangulation {

	class triangulator
	{
		public:
			triangulator(triangulation::logger *pLogger);
			bool addTriangle(triangulation::point &p1, triangulation::point &p2, triangulation::point &p3, double a);
			void addArea(triangulation::area ar);
			std::vector<triangulation::area> &getAreas();
			std::vector<triangulation::triangle> &getTriangles();
			void triangulate();
			void clear_triangles();
			void clear_areas();
		private:
			std::vector<triangulation::area> m_areas;
			std::vector<triangulation::triangle> m_triangles;
			int nCurrArea;
			triangulation::logger *m_pLogger;

			//
			bool findTriangle(triangulation::point p1, triangulation::point p2, triangulation::point p3);
			bool findNearPoint(triangulation::point p, triangulation::point &result, double r);

			bool addTriangleAsIs(triangulation::point &p1, triangulation::point &p2, triangulation::point &p3, bool &bShouldChange_p3, double a);
			bool hasCurrentArea(triangulation::point p1);
			bool hasIntersection(const triangulation::point &p1, const triangulation::point &p2, const triangulation::point &p3, triangulation::triangle &result);
	};

	bool recours_netting(triangulation::point &p1, triangulation::point &p2, double h, double a, triangulation::triangulator *tr);
}

#endif
