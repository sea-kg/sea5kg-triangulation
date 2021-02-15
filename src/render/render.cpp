#include "render.h"
#include "wsjcpp_core.h"
#include <cmath>
#define PI 3.14159265

// ---------------------------------------------------------------------
// RenderColor

RenderColor::RenderColor(int nR, int nG, int nB, int nA) {
    m_nR = nR;
    m_nG = nG;
    m_nB = nB;
    m_nA = nA;
}

void RenderColor::changeRenderColor(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_nR, m_nG, m_nB, m_nA);
}

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

RenderLine::RenderLine(const CoordXY &p1, const CoordXY &p2, const RenderColor &color, int nPositionZ) 
: m_color(color), RenderObject(nPositionZ) {
    m_coord1 = p1;
    m_coord2 = p2;
    m_startCoord1 = p1;
    m_startCoord2 = p2;
}

void RenderLine::modify(const AppState& state) {
    m_coord1 = state.getCoordLeftTop() + m_startCoord1;
    m_coord2 = state.getCoordLeftTop() + m_startCoord2;
}

void RenderLine::draw(SDL_Renderer* renderer) {
    m_color.changeRenderColor(renderer);
    SDL_RenderDrawLine(renderer, m_coord1.x(), m_coord1.y(), m_coord2.x(), m_coord2.y());
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
    const RenderColor &color,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_line1(p1,p2,color),
    m_line2(p2,p3,color),
    m_line3(p3,p1,color),
    m_color(color)
{
    int nMiddleX = (p1.x() + p2.x() + p3.x())/3;
    int nMiddleY = (p1.y() + p2.y() + p3.y())/3;
    m_middlePoint = CoordXY(nMiddleX, nMiddleY);
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
    m_color.changeRenderColor(renderer);

    SDL_Rect srcrect;
    srcrect.x = m_middlePoint.x()-2;
    srcrect.y = m_middlePoint.y()-2;
    srcrect.w = 4;
    srcrect.h = 4;
    SDL_RenderFillRect(renderer, &srcrect);
}

// ---------------------------------------------------------------------
// RenderMouse

RenderMouse::RenderMouse(
    const CoordXY &p1,
    const RenderColor &color,
    int nPositionZ
) : RenderObject(nPositionZ),
    m_p1(p1),
    m_color(color)
{
    CoordXY p2 = p1;
    m_pDiff = CoordXY(10,10);
    p2 += m_pDiff;
    m_pLine1 = new RenderLine(p1, p2, color);
    // int nMiddleX = (p1.x() + p2.x() + p3.x())/3;
    // int nMiddleY = (p1.y() + p2.y() + p3.y())/3;
    // m_middlePoint = CoordXY(nMiddleX, nMiddleY);
}

void RenderMouse::modify(const AppState& state) {
    m_pLine1->modify(state);
    // m_line2.modify(state);
    // m_line3.modify(state);
}

void RenderMouse::draw(SDL_Renderer* renderer) {
    // m_color.changeRenderColor(renderer);
    m_pLine1->draw(renderer);
    // m_line2.draw(renderer);
    // m_line3.draw(renderer);
    

    // SDL_Rect srcrect;
    // srcrect.x = m_middlePoint.x()-2;
    // srcrect.y = m_middlePoint.y()-2;
    // srcrect.w = 4;
    // srcrect.h = 4;
    // SDL_RenderFillRect(renderer, &srcrect);
}

void RenderMouse::updateCoord(int nX, int nY) {
    CoordXY p1(nX,nY);
    CoordXY p2 = p1;
    p2 += m_pDiff;
    m_pLine1->updateAbsoluteCoords(p1,p2);
}

// ---------------------------------------------------------------------
// RenderTextBlock

RenderAbsoluteTextBlock::RenderAbsoluteTextBlock(const CoordXY &p0, const std::string &sText, int nPositionZ)
: RenderObject(nPositionZ) {
    m_coordCenter = p0;
    m_sText = sText;
    m_sUpdateText = m_sText;
    m_pFont = TTF_OpenFont("./res/fonts/roboto/Roboto-Black.ttf", 18);
    m_color = {255, 255, 255};
    if (!m_pFont) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }

    // SDL_Rect currentFrame;
}

void RenderAbsoluteTextBlock::modify(const AppState& state) {
    // nothing i think
    if (m_sUpdateText != m_sText) {
        m_sText = m_sUpdateText;
    }
};

void RenderAbsoluteTextBlock::draw(SDL_Renderer* renderer) {
    // int w,h;
    // if (TTF_SizeText(font,"Hello World!",&w,&h)) {
        // perhaps print the current TTF_GetError(), the string can't be rendered...
    // } else {
    //    printf("width=%d height=%d\n",w,h);
    // }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_pFont, m_sText.c_str(), m_color);
    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = m_coordCenter.x();  //controls the rect's x coordinate 
    Message_rect.y = m_coordCenter.y(); // controls the rect's y coordinte
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect

    // Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    // Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
};

void RenderAbsoluteTextBlock::updateText(const std::string &sNewText) {
    m_sUpdateText = sNewText;
}

// ---------------------------------------------------------------------
// RenderButton

RenderButton::RenderButton(
    const CoordXY &p1,
    const std::string &sText,
    const RenderColor &color,
    int nPositionZ
) : RenderObject(nPositionZ), m_color(color), m_point(p1) {
    m_sText = sText;
}

void RenderButton::modify(const AppState& state) {
    
}
void RenderButton::draw(SDL_Renderer* renderer) {

}
