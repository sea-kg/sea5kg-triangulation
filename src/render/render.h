#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "app_state.h"
#include <vector>
#include <string>

class RenderColor {
    public:
        RenderColor(int nR, int nG, int nB, int nA);
        void changeRenderColor(SDL_Renderer* renderer);

    private:
        int m_nR;
        int m_nG;
        int m_nB;
        int m_nA;
};

class RenderObject {

    public:
        RenderObject(int nPositionZ);
        int getPositionZ();
        virtual void modify(const AppState& state);
        virtual void draw(SDL_Renderer* pRenderer) = 0;

    private:
        int m_nPositionZ;
};

class RenderLine : public RenderObject {

    public:
        RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ = 0);
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        
        const CoordXY &getAbsoluteCoord1();
        const CoordXY &getAbsoluteCoord2();
        const CoordXY &getCoord1();
        const CoordXY &getCoord2();
        void updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2);

    private:
        RenderColor m_color;
        CoordXY m_startCoord1;
        CoordXY m_startCoord2;
        CoordXY m_coord1;
        CoordXY m_coord2;
};

class RenderRect : public RenderObject {

    public:
        RenderRect(const CoordXY &p1, int w, int h, int nPositionZ = 0);
        virtual void modify(const AppState& state) override;
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
            const RenderColor &color = RenderColor(255,255,255,255),
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        RenderLine m_line1;
        RenderLine m_line2;
        RenderLine m_line3;
        RenderColor m_color;
        CoordXY m_middlePoint;
};

class RenderAbsoluteTextBlock : public RenderObject {

    public:
        RenderAbsoluteTextBlock(
            const CoordXY &p0,
            const std::string &sText,
            int nPositionZ = 0
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;
        void updateText(const std::string &sNewText);

    private:
        CoordXY m_coordCenter;
        std::string m_sText;
        std::string m_sUpdateText;
        TTF_Font* m_pFont;
        SDL_Color m_color;

        SDL_Rect currentFrame;
};


class RenderButton : public RenderObject {

    public:
        RenderButton(
            const CoordXY &p1,
            const std::string &sText,
            const RenderColor &color = RenderColor(255,255,255,255),
            int nPositionZ = 1000
        );
        virtual void modify(const AppState& state) override;
        virtual void draw(SDL_Renderer* renderer) override;

    private:
        std::string m_sText;
        CoordXY m_point;
        // RenderRect m_rect;
        RenderColor m_color;
};