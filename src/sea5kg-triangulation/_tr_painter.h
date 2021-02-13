#pragma once

#include "_tr_triangulation.h"
#include "_tr_triangulator.h"

class painter
{
    public:
        // painter(TImage *img, triangulation::logger *pLogger);
        // void addArea(Sea5kgTriangulationArea ar);

        // void repaintArea(Sea5kgTriangulationArea &ar);

        void refresh();
        void triangulate();

        // new
        void addToBufferArea(int X, int Y);
        void applyBufferArea();
        void clear();
        void clear_buffer();
        bool hasAreas();

        // void SaveAreas(UnicodeString filename);
        // void LoadAreas(UnicodeString filename);
        std::vector<Sea5kgTriangulationTriangle> &getTriangles();
        std::vector<Sea5kgTriangulationArea> &getAreas();

        bool getMinMaxXY(int &maxX, int &minX, int &maxY, int &minY);
    private:

        // new
        Sea5kgTriangulationArea bufferArea;

        // old
        bool addNewTriangle(
            std::vector<Sea5kgTriangulationTriangle> &vTriangles,
            Sea5kgTriangulationPoint &p1_buff,
            const Sea5kgTriangulationPoint &p2,
            Sea5kgTriangulationPoint &p4_buff,
            Sea5kgTriangulationPoint &p5_buff,
            double r
        );

        triangulation::triangulator *m_pTriangulator;
        // std::vector<Sea5kgTriangulationArea> m_areas;
};
