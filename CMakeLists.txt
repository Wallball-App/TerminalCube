cmake_minimum_required(VERSION 3.25)
project(TerminalCube)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
include(FetchContent)

set(EIGEN_BUILD_DOC OFF CACHE INTERNAL "")
set(EIGEN_BUILD_TESTS OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "")

set(BUILD_APP "Builds the Application (tcube.exe)" ON)

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

add_library(TCube STATIC src/Cube.cpp src/Algorithm_Reader.cpp)
target_include_directories(TCube PUBLIC 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)

target_link_libraries(TCube PUBLIC eigen fmt)
target_compile_features(TCube PRIVATE cxx_std_23)

if(BUILD_APP) 
    add_executable(tcube src/main.cpp)
    target_link_libraries(tcube PRIVATE TCube)
    target_compile_features(tcube PRIVATE cxx_std_23)
endif()
