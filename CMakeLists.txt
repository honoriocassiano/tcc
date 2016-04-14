cmake_minimum_required(VERSION 2.8)
project(TCC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES
    "src/*.hpp"
    "src/*.h"
    "src/*.cpp"
    "src/*.C"
    "src/*.c"
)

add_executable(TCC ${SOURCE_FILES})

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/structures)
include_directories(${CMAKE_SOURCE_DIR}/src/structures/Array)
include_directories(${CMAKE_SOURCE_DIR}/src/structures/Halfedge)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})


target_link_libraries(TCC ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})
