cmake_minimum_required(VERSION 3.10)
project(TallerColor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/ejercicio1_rgb_hsv.cpp
    src/ejercicio2_saturacion.cpp
    src/ejercicio3_kmeans.cpp
    src/ejercicio4_gray_world.cpp
    src/ejercicio6_gamma.cpp
    src/ejercicio7_vignette.cpp
    src/utils.cpp
)

add_executable(taller ${SOURCES})

target_link_libraries(taller ${OpenCV_LIBS})

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")
