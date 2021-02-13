#include <iostream>
#include <vector>
#include <fstream>

#include "render_window.h"
#include "wsjcpp_core.h"


int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cerr << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    int nWindowWidth = 1280;
    int nWindowHeight = 720;

    GameState stateObjects(nWindowWidth, nWindowHeight);

    RenderWindow window(
        "Triangulation (v0.0.1)",
        stateObjects.windowWidth(),
        stateObjects.windowHeight()
    );

    // load map from json
    // std::ifstream ifs("./res/data.json");
    // nlohmann::json jf = nlohmann::json::parse(ifs);

    /*nlohmann::json jsonBuildings = jf["buildings"];
    for (auto it = jsonBuildings.begin(); it != jsonBuildings.end(); ++it) {
        // std::cout << it.key() << " | " << it.value() << "\n";
        GameBuilding *pBuilding = new GameBuilding(it.value());
        stateObjects.addBuilding(pBuilding);
        // window.addObject(new RenderBuilding2(pBuilding, buildingTexture));
    }
    */

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