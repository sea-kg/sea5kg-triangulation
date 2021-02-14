#include "render.h"
#include "wsjcpp_core.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderObject

RenderObject::RenderObject(int nPositionZ) {
    m_nPositionZ = nPositionZ;
}

int RenderObject::getPositionZ() {
    return m_nPositionZ;
}

void RenderObject::modify(const AppState& state) {
    // nothing modify in base object
}

// ---------------------------------------------------------------------
// RenderLine

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, int nPositionZ) 
: RenderObject(nPositionZ) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
    m_nR = 255;
    m_nG = 255;
    m_nB = 255;
    m_nA = 255;
}

void RenderLine::modify(const AppState& state) {
    m_coord1 = state.getCoordLeftTop() + m_startCoord1;
    m_coord2 = state.getCoordLeftTop() + m_startCoord2;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
}

void RenderLine::setColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

const CoordXY &RenderLine::getAbsoluteCoord1() {
    return m_startCoord1;
}

const CoordXY &RenderLine::getAbsoluteCoord2() {
    return m_startCoord2;
}

const CoordXY &RenderLine::getCoord1() {
    return m_coord1;
}

const CoordXY &RenderLine::getCoord2() {
    return m_coord2;
}

void RenderLine::updateAbsoluteCoords(const CoordXY &p1, const CoordXY &p2) {
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

// ---------------------------------------------------------------------
// RenderRect

RenderRect::RenderRect(const CoordXY &p1, int w, int h, int nPositionZ)
: RenderObject(nPositionZ) {
    m_coord1 = p1;
    m_nW = w;
    m_nH = h;
}

void RenderRect::modify(const AppState& state) {
    // nothing
}

void RenderRect::draw(SDL_Renderer* renderer) {
    SDL_Rect srcrect;
    srcrect.x = m_coord1.x();
    srcrect.y = m_coord1.y();
    srcrect.w = m_nW;
    srcrect.h = m_nH;
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
    SDL_RenderFillRect(renderer, &srcrect);
}
        
void RenderRect::setColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

// ---------------------------------------------------------------------
// RenderTriangle

RenderTriangle::RenderTriangle(
    const CoordXY &p1,
    const CoordXY &p2,
    const CoordXY &p3,
    int nPositionZ
) : RenderObject(nPositionZ), m_line1(p1,p2), m_line2(p2,p3), m_line3(p3,p1) {
    int nMiddleX = (p1.x() + p2.x() + p3.x())/3;
    int nMiddleY = (p1.y() + p2.y() + p3.y())/3;
    m_middlePoint = CoordXY(nMiddleX, nMiddleY);
}

void RenderTriangle::setColor(int nR, int nG, int nB, int nA) {
    m_line1.setColor(nR,nG,nB,nA);
    m_line2.setColor(nR,nG,nB,nA);
    m_line3.setColor(nR,nG,nB,nA);

    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

void RenderTriangle::modify(const AppState& state) {
    m_line1.modify(state);
    m_line2.modify(state);
    m_line3.modify(state);
}

void RenderTriangle::draw(SDL_Renderer* renderer) {
    m_line1.draw(renderer);
    m_line2.draw(renderer);
    m_line3.draw(renderer);

    SDL_Rect srcrect;
    srcrect.x = m_middlePoint.x()-2;
    srcrect.y = m_middlePoint.y()-2;
    srcrect.w = 4;
    srcrect.h = 4;
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
    SDL_RenderFillRect(renderer, &srcrect);
}