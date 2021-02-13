#pragma once
#include <SDL.h>
#include "game.h"
#include <vector>
#include <string>

class RenderObject {

    public:
        RenderObject(int nPositionZ);
        int getPositionZ();
        virtual void modify(const GameState& state);
        virtual void draw(SDL_Renderer* pRenderer) = 0;

    private:
        int m_nPositionZ;
};

class RenderLine : public RenderObject {

    public:
        RenderLine(const CoordXY &p1, const CoordXY &p2, int nPositionZ = 0);
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        
        void setColor(int nR, int nG, int nB, int nA);
        const CoordXY &getAbsoluteCoord1();
        const CoordXY &getAbsoluteCoord2();
        const CoordXY &getCoord1();
        const CoordXY &getCoord2();
        void updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2);

    private:
        CoordXY m_startCoord1;
        CoordXY m_startCoord2;
        CoordXY m_coord1;
        CoordXY m_coord2;

        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};

class RenderRect : public RenderObject {

    public:
        RenderRect(const CoordXY &p1, int w, int h, int nPositionZ = 0);
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        
        void setColor(int nR, int nG, int nB, int nA);

    private:
        CoordXY m_coord1;
        int m_nW;
        int m_nH;

        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};

class RenderTriangle : public RenderObject {

    public:
        RenderTriangle(
            const CoordXY &p1,
            const CoordXY &p2,
            const CoordXY &p3,
            int nPositionZ = 0
        );
        void setColor(int nR, int nG, int nB, int nA);
        virtual void modify(const GameState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        CoordXY m_middlePoint;

        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};
