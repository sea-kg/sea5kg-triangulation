//---------------------------------------------------------------------------

#ifndef _tr_painterH
#define _tr_painterH
//---------------------------------------------------------------------------
#include "_tr_triangulation.h"
#include "_tr_triangulator.h"

class painter
{
	public:
		painter(TImage *img, triangulation::logger *pLogger);
		// void addArea(triangulation::area ar);

		// void repaintArea(triangulation::area &ar);

		void refresh();
		void triangulate();
		void triangulate2();
		void triangulate3();

		// new
		void addToBufferArea(int X, int Y);
		void applyBufferArea();
		void clear();
		void clear_buffer();
		bool hasAreas();

		void SaveAreas(UnicodeString filename);
		void LoadAreas(UnicodeString filename);

	private:

		// new
		triangulation::area bufferArea;

		// old
		bool addNewTriangle(
			std::vector<triangulation::triangle> &vTriangles,
			triangulation::point &p1_buff,
			const triangulation::point &p2,
			triangulation::point &p4_buff,
			triangulation::point &p5_buff,
			double r
		);

		/*triangulation::point findNearPoint(std::vector<triangulation::triangle> &vTriangles, triangulation::point p, double r);
		triangulation::point paint(std::vector<triangulation::triangle> &vTriangles);
		bool addToTriangles(std::vector<triangulation::triangle> &vTriangles, triangulation::point &p1, triangulation::point &p2, triangulation::point &p3, double r);
		bool findTriangle(std::vector<triangulation::triangle> &vTriangles, triangulation::point &p1, triangulation::point &p2, triangulation::point &p3);
		bool hasIntersection(std::vector<triangulation::triangle> &vTriangles, triangulation::point &p1, triangulation::point &p2, triangulation::point &p3);

		bool findNearPointWithAreaSide(const triangulation::point &p, triangulation::point &res, double len);

		triangulation::point getMiddlePoint(const triangulation::area &area);
		double getSquare(const triangulation::area &area);

		void painter::recurs(
			std::vector<triangulation::triangle> &vTriangles,
			triangulation::area &area,
			triangulation::point p1,
			triangulation::point p2,
			double h,
			double a
		);

		void triangulate_area2(const triangulation::area &area);
		void triangulate_area3(triangulation::area &area);
		*/

		TImage * m_img;
		triangulation::logger *m_pLogger;
		triangulation::triangulator *m_pTriangulator;
		// std::vector<triangulation::area> m_areas;
};

#endif
