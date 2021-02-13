#pragma once

#include "sea5kg_triangulation.h"
#include "_tr_area.h"
#include <vector>
#include <list>

//---------------------------------------------------------------------------
namespace triangulation {

    class triangulator
    {
        public:
            triangulator();
            bool addTriangle( 
                    Sea5kgTriangulationPoint p1, 
                    Sea5kgTriangulationPoint p2, 
                    Sea5kgTriangulationPoint p3, 
                    double a, 
                    std::vector<Sea5kgTriangulationLine> &m_lines
            );

            bool addTriangleAsIs(
                Sea5kgTriangulationPoint p1, 
                Sea5kgTriangulationPoint p2, 
                Sea5kgTriangulationPoint p3, 
                double a,
                std::vector<Sea5kgTriangulationLine> &m_lines
            );

            void addArea(triangulation::area ar);
            std::vector<triangulation::area> &getAreas();
            std::vector<Sea5kgTriangulationTriangle> &getTriangles();
            void triangulate();

            void triangulate_add_triangles();
            void triangulate_resizing();
            void triangulate_remove_triangles();

            void clear_triangles();
            void clear_areas();
        private:
            
            std::vector<triangulation::area> m_areas;
            std::vector<Sea5kgTriangulationTriangle> m_triangles;
            int m_nCurrArea;
            int m_nDivisor;
            double m_nInfelicity;
    
            // triangulation::logger *m_pLogger;
            void step_first();
            void step_first_alien();

            void findPoints(
                const Sea5kgTriangulationPoint &p1, 
                const std::vector<Sea5kgTriangulationPoint> &points, 
                double a, 
                std::vector<Sea5kgTriangulationTriangle> &triangles
            );


            //
            bool findTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3);
            bool findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r);
                        bool findNearPointSide(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r);

            bool hasCurrentArea(Sea5kgTriangulationPoint p1);
            bool hasCurrentArea(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2);
            bool hasIntersectionWithOtherTriangles(
                const Sea5kgTriangulationTriangle &tr,  
                Sea5kgTriangulationTriangle &result,
                Sea5kgTriangulationPoint &p_result
            );

            void fillArray(const Sea5kgTriangulationPoint &p, std::vector<Sea5kgTriangulationPoint> &p_arr, double r);
    };


    class netting
    {
        public:
            netting(const Sea5kgTriangulationPoint &p1, const Sea5kgTriangulationPoint &p2, double h, double a, triangulation::triangulator *tr);
            void processing();
            void calcPoints(const Sea5kgTriangulationLine &L, Sea5kgTriangulationPoint &p4, Sea5kgTriangulationPoint &p5);
        private:
          std::vector<Sea5kgTriangulationLine> m_lines;
          double m_h; 
          double m_a; 
          triangulation::triangulator *m_tr;
    };

//    bool recours_netting(Sea5kgTriangulationPoint &p1, Sea5kgTriangulationPoint &p2, double h, double a, triangulation::triangulator *tr);
}