#pragma once

#include "_tr_point.h"
#include "_tr_triangle.h"
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
                    triangulation::point p1, 
                    triangulation::point p2, 
                    triangulation::point p3, 
                    double a, 
                    std::vector<triangulation::line> &m_lines
            );

            bool addTriangleAsIs(
                triangulation::point p1, 
                triangulation::point p2, 
                triangulation::point p3, 
                double a,
                std::vector<triangulation::line> &m_lines
            );

            void addArea(triangulation::area ar);
            std::vector<triangulation::area> &getAreas();
            std::vector<triangulation::triangle> &getTriangles();
            void triangulate();

            void triangulate_add_triangles();
            void triangulate_resizing();
            void triangulate_remove_triangles();

            void clear_triangles();
            void clear_areas();
        private:
            
            std::vector<triangulation::area> m_areas;
            std::vector<triangulation::triangle> m_triangles;
            int m_nCurrArea;
            int m_nDivisor;
            double m_nInfelicity;
    
            // triangulation::logger *m_pLogger;
            void step_first();
          void step_first_alien();

            void findPoints(
                const triangulation::point &p1, 
                const std::vector<triangulation::point> &points, 
                double a, 
                std::vector<triangulation::triangle> &triangles
            );


            //
            bool findTriangle(triangulation::point p1, triangulation::point p2, triangulation::point p3);
            bool findNearPoint(triangulation::point p, triangulation::point &result, double r);
                        bool findNearPointSide(triangulation::point p, triangulation::point &result, double r);

            bool hasCurrentArea(triangulation::point p1);
            bool hasCurrentArea(triangulation::point p1, triangulation::point p2);
            bool hasIntersectionWithOtherTriangles(
                const triangulation::triangle &tr,  
                triangulation::triangle &result,
                triangulation::point &p_result
            );

            void fillArray(const triangulation::point &p, std::vector<triangulation::point> &p_arr, double r);
    };


    class netting
    {
        public:
            netting(const triangulation::point &p1, const triangulation::point &p2, double h, double a, triangulation::triangulator *tr);
            void processing();
            void calcPoints(const triangulation::line &L, triangulation::point &p4, triangulation::point &p5);
        private:
          std::vector<triangulation::line> m_lines;
          double m_h; 
          double m_a; 
          triangulation::triangulator *m_tr;
    };

//    bool recours_netting(triangulation::point &p1, triangulation::point &p2, double h, double a, triangulation::triangulator *tr);
}