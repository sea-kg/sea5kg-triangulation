#include <iostream>
#include <vector>
#include <fstream>

#include "render_window.h"
#include "wsjcpp_core.h"
#include "sea5kg_triangulation.h"


int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    int nWindowWidth = 1280;
    int nWindowHeight = 720;

    Sea5kgTriangulationTriangulator *pTriangulator = new Sea5kgTriangulationTriangulator();

    GameState stateObjects(nWindowWidth, nWindowHeight);

    RenderWindow window(
        "Triangulation (v0.0.1)",
        stateObjects.windowWidth(),
        stateObjects.windowHeight()
    );

    // load map from json
    std::ifstream ifs("./input.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);

    nlohmann::json jsonAreas = jf["areas"];
    for (auto& el : jsonAreas.items()) {
        std::string sAreaId = el.value()["id"];
        int nNumberOfTriangles = el.value()["number-of-triangels"];
        std::cout << "Load area " << sAreaId << std::endl;
        Sea5kgTriangulationArea ar(sAreaId);
        ar.setCountTriangles(nNumberOfTriangles);
        nlohmann::json jsonPoints = el.value()["points"];
        CoordXY prev;
        CoordXY next;
        CoordXY first;
        int nCount = 0;
        for (auto& pt : jsonPoints.items()) {
            int nX = pt.value()["x"];
            int nY = pt.value()["y"];
            ar.addPoint(nX, nY);
            CoordXY next(nX,nY);
            if (nCount == 0) {
                prev = next;
                first = next;
            } else {
                window.addObject(new RenderLine(prev, next));
                prev = next;
            }
            nCount++;
        }
        window.addObject(new RenderLine(prev, first));

        pTriangulator->addArea(ar);
    }

    int nCenterX = nWindowWidth/2;
    int nCenterY = nWindowHeight/2;
    CoordXY coordCenter(nCenterX, nCenterY);
    
    
    // object
    // window.addObject(new RenderTriangle(
    //     CoordXY(320, 200),
    //     CoordXY(300, 240),
    //     CoordXY(340, 240)
    // ));
    window.sortObjectsByPositionZ();

    bool gameRunning = true;

    SDL_Event event;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    stateObjects.init();
    while (gameRunning) {

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  stateObjects.incrementCoordLeftTopX(5); break;
                    case SDLK_RIGHT: stateObjects.incrementCoordLeftTopX(-5); break;
                    case SDLK_UP:    stateObjects.incrementCoordLeftTopY(5); break;
                    case SDLK_DOWN:  stateObjects.incrementCoordLeftTopY(-5); break;
                    case SDLK_w: stateObjects.incrementCoordLeftTopY(5); break;
                    case SDLK_s: stateObjects.incrementCoordLeftTopY(-5); break;
                    case SDLK_a:  stateObjects.incrementCoordLeftTopX(5); break;
                    case SDLK_d: stateObjects.incrementCoordLeftTopX(-5); break;
                }
                std::cout << "SDL_KEYDOWN" << std::endl;
            }
        }

        stateObjects.updateElapsedTime();
        window.clear();
        window.modifyObjects(stateObjects);
        window.drawObjects();

        // FPS
        nNumberOfFrames++;
        nElapsed = WsjcppCore::getCurrentTimeInMilliseconds() - nStartTime;
        if (nElapsed > 3000) {
            double nFPS = nNumberOfFrames;
            nFPS /= nElapsed;
            nFPS *= 1000;
            std::cout << "FPS: " << nFPS << std::endl;
            nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
            nNumberOfFrames = 0;
        }
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}