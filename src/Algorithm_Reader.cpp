#include <Eigen/Dense>

#include <iostream>
#include <string>
#include <vector>

#include "Cube.h"
#include "Algorithm_Reader.h"


void Algorithm_Reader::parse(std::string alg, Cube& cube) {
    std::vector<std::string> moves;
    std::string delimiter = " ";
    size_t pos = 0;
    size_t next = 0;

    while((next = alg.find(delimiter, pos)) != std::string::npos) {
        moves.push_back(alg.substr(pos, next-pos).c_str());
        pos = next + delimiter.length();
    }
    moves.push_back(alg.substr(pos).c_str());

    for(std::string move : moves) {
        if(move == "R") {
            cube.rotateFace(Enums::FaceEnum::RIGHT, 1);
        }
        if(move == "R'") {
            cube.rotateFace(Enums::FaceEnum::RIGHT, -1);
        }
        if(move == "R2") {
            cube.rotateFace(Enums::FaceEnum::RIGHT, 2);
        }

        if(move == "L") {
            cube.rotateFace(Enums::FaceEnum::LEFT, 1);
        }
        if(move == "L'") {
            cube.rotateFace(Enums::FaceEnum::LEFT, -1);
        }
        if(move == "L2") {
            cube.rotateFace(Enums::FaceEnum::LEFT, 2);
        }

        if(move == "U") {
            cube.rotateFace(Enums::FaceEnum::TOP, 1);
        }
        if(move == "U'") {
            cube.rotateFace(Enums::FaceEnum::TOP, -1);
        }
        if(move == "U2") {
            cube.rotateFace(Enums::FaceEnum::TOP, 2);
        }

        if(move == "D") {
            cube.rotateFace(Enums::FaceEnum::BOTTOM, 1);
        }
        if(move == "D'") {
            cube.rotateFace(Enums::FaceEnum::BOTTOM, -1);
        }
        if(move == "D2") {
            cube.rotateFace(Enums::FaceEnum::BOTTOM, 2);
        }

        if(move == "F") {
            cube.rotateFace(Enums::FaceEnum::FRONT, 1);
        }
        if(move == "F'") {
            cube.rotateFace(Enums::FaceEnum::FRONT, -1);
        }
        if(move == "F2") {
            cube.rotateFace(Enums::FaceEnum::FRONT, 2);
        }

        if(move == "B") {
            cube.rotateFace(Enums::FaceEnum::BACK, 1);
        }
        if(move == "B'") {
            cube.rotateFace(Enums::FaceEnum::BACK, -1);
        }
        if(move == "B2") {
            cube.rotateFace(Enums::FaceEnum::BACK, 2);
        }
        if(move == "reset") {
            cube.reset();
        }
    }
}