cmake_minimum_required(VERSION 3.7)

project(nbodies)

set(CMAKE_CXX_STANDARD 20)

IF (WIN32)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2/")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
ELSE()
    # set stuff for other systems
ENDIF()


find_package(OpenMP)
find_package(OpenGL)

find_package(SDL2)

include_directories(nbodies ${SDL2_INCLUDE_DIRS})

add_executable(nbodies main.cpp body.h graphics.cpp graphics.h simulation.cpp simulation.h tester.cpp tester.h)

target_link_libraries(nbodies PUBLIC OpenMP::OpenMP_CXX ${SDL2_LIBRARIES} PRIVATE "-framework OpenGL")