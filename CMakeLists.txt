cmake_minimum_required(VERSION 3.7)

project(sea5kg-triangulation)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${sea5kg-triangulation_SOURCE_DIR})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/game")
list (APPEND WSJCPP_INCLUDE_DIRS "src/render")

# source files
list (APPEND WSJCPP_SOURCES "src/main.cpp")
list (APPEND WSJCPP_SOURCES "src/app_state.h")
list (APPEND WSJCPP_SOURCES "src/app_state.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render.h")
list (APPEND WSJCPP_SOURCES "src/render/render.cpp")
list (APPEND WSJCPP_SOURCES "src/render/render_window.h")
list (APPEND WSJCPP_SOURCES "src/render/render_window.cpp")
list (APPEND WSJCPP_SOURCES "src/sea5kg_triangulation.cpp")
list (APPEND WSJCPP_SOURCES "src/sea5kg_triangulation.h")

############################
# SDL2
find_package(SDL2 REQUIRED)
if ( SDL2_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
    list (APPEND WSJCPP_LIBRARIES ${SDL2_LIBRARIES})
endif( SDL2_FOUND )
############################

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (sea5kg-triangulation ${WSJCPP_SOURCES})

target_link_libraries(sea5kg-triangulation ${WSJCPP_LIBRARIES} )
