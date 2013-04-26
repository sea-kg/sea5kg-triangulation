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

		// new
		void addToBufferArea(int X, int Y);
		void applyBufferArea();
		void clear();
		void clear_buffer();
		bool hasAreas();

		void SaveAreas(UnicodeString filename);
		void LoadAreas(UnicodeString filename);
		std::vector<triangulation::triangle> &getTriangles();
		std::vector<triangulation::area> &getAreas();

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

		TImage * m_img;
		triangulation::logger *m_pLogger;
		triangulation::triangulator *m_pTriangulator;
		// std::vector<triangulation::area> m_areas;
};

#endif
