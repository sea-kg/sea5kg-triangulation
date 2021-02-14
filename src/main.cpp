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

    AppState appState(nWindowWidth, nWindowHeight);

    RenderWindow window(
        "sea5kg-triangulation (v0.0.2)",
        appState.windowWidth(),
        appState.windowHeight()
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
        for (auto& pt : jsonPoints.items()) {
            int nX = pt.value()["x"];
            int nY = pt.value()["y"];
            ar.addPoint(nX, nY);
        }
        pTriangulator->addArea(ar);
    }

    pTriangulator->triangulate();


    // add to renderer
    RenderColor areaColor(255,255,255,255);

    const std::vector<Sea5kgTriangulationArea> &vAreas = pTriangulator->getAreas();
    for (int i = 0; i < vAreas.size(); i++) {
        Sea5kgTriangulationArea ar = vAreas[i];
        int nSize = ar.count();
        CoordXY prev;
        CoordXY first;
        for (int x = 0; x < ar.count(); x++) {
            CoordXY next(ar.getPoint(x).getX(),ar.getPoint(x).getY());
            if (x == 0) {
                prev = next;
                first = next;
            } else {
                RenderLine *pLine = new RenderLine(prev, next, areaColor);
                window.addObject(pLine);
                prev = next;
            }
            RenderRect *pRect = new RenderRect(CoordXY(next.x() - 4, next.y() - 4), 8, 8);
            pRect->setColor(255,255,255,255);
            window.addObject(pRect);
        }
        RenderLine *pLine = new RenderLine(prev, first, areaColor);
        window.addObject(pLine);
    }

    RenderColor triangleColor(0,255,255,190);

    const std::vector<Sea5kgTriangulationTriangle> &vTriangles = pTriangulator->getTriangles();
    for (int i = 0; i < vTriangles.size(); i++) {
        Sea5kgTriangulationTriangle tr = vTriangles[i];
        auto *pTriangle = new RenderTriangle(
            CoordXY(tr.p1.getX(), tr.p1.getY()),
            CoordXY(tr.p2.getX(), tr.p2.getY()),
            CoordXY(tr.p3.getX(), tr.p3.getY()),
            triangleColor
        );
        window.addObject(pTriangle);
    }

    window.sortObjectsByPositionZ();

    bool appRunning = true;

    SDL_Event event;

    long nNumberOfFrames = 0;
    long nStartTime = WsjcppCore::getCurrentTimeInMilliseconds();
    long nElapsed = 0;
    appState.init();
    while (appRunning) {

        // Get our controls and events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                appRunning = false;
            }
        }

        appState.updateElapsedTime();
        window.clear();
        window.modifyObjects(appState);
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