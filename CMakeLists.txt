cmake_minimum_required(VERSION 3.50)
project(TerminalCube)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

set(EIGEN_BUILD_DOC OFF CACHE INTERNAL "")
set(EIGEN_BUILD_TESTS OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "")

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 5.0.0
)
FetchContent_Declare(
    fmt 
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git 
    GIT_TAG 12.2.0
)
FetchContent_MakeAvailable(eigen)
FetchContent_MakeAvailable(fmt)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(TCube STATIC src/Cube.cpp src/Algorithm_Reader.cpp)
target_include_directories(TCube PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TCube PUBLIC eigen fmt)

add_executable(tcube src/main.cpp)
target_link_libraries(tcube PRIVATE TCube)
