#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numbers>

#include <fmt/core.h>
#include <fmt/color.h>

#include "Cube.h"

using namespace pieces;
using namespace Enums;

int cubesize;
int minpoint;
int maxpoint;
float midpoint;

const char* PIECE_CHAR = "O";
const char* PIECE_SEPARATOR = "";
constexpr const char* FACE_SEPARATOR = "    ";

Cube::Cube(int size) {
    std::cout<<"Initializing Cube with size: "<<size<<std::endl;
    cubesize = size;
    Pieces.resize(size);
    for(int i = 0; i < size; ++i) {
        Pieces[i].resize(size);
        for(int j = 0; j < size; ++j) {
            Pieces[i][j].resize(size);
        }
    }
    array<float, 4> values = {(float)size, (size%2 == 0) ? -size/2.0f : -(size-1)/2.0f, (size%2 == 0) ? size/2.0f : (size-1)/2.0f, size/2.0f};
    size = (int)values[0];
    minpoint = (int)values[1];
    maxpoint = (int)values[2];
    midpoint = values[3];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                const bool i0 = (i == 0 || i == size - 1);
                const bool j0 = (j == 0 || j == size - 1);
                const bool k0 = (k == 0 || k == size - 1);

                Eigen::Vector3f position;
                if(size % 2 == 0) {
                    position = Eigen::Vector3f(static_cast<float>(i - size/2), static_cast<float>(j - size/2), static_cast<float>(k - size/2));
                } else if(size % 2 == 1) {
                    position = Eigen::Vector3f(static_cast<float>(i - (size-1)/2), static_cast<float>(j - (size-1)/2), static_cast<float>(k - (size-1)/2));
                }

                std::unique_ptr<pieces::Piece> piece;
                if (i0 + j0 + k0 == 3) {
                    piece = std::make_unique<Corner>(values, position, position);
                    static_cast<Corner*>(piece.get())->init();
                } else if (i0 + j0 + k0 == 2) {
                    piece = std::make_unique<Edge>(values, position, position);
                    static_cast<Edge*>(piece.get())->init();
                } else if (i0 + j0 + k0 == 1) {
                    piece = std::make_unique<Center>(values, position, position);
                    static_cast<Center*>(piece.get())->init();
                } else {
                    continue;
                }
                Pieces[i][j][k] = std::move(piece);
            }
        }
    }
}
Eigen::Matrix3f Cube::buildRotationMatrix(Eigen::Vector3f axis, float angle) {
    Eigen::Matrix3f rotationMatrix;
    float c = cos(angle); //RADIANS
    float s = sin(angle);
    float t = 1 - c;

    if(axis == Eigen::Vector3f(1, 0, 0)) { //REVERSED
        rotationMatrix << 1, 0, 0,
                          0, c, s,
                          0, -s, c;
    } else if(axis == Eigen::Vector3f(-1, 0, 0)) {
        rotationMatrix << 1, 0, 0,
                          0, c, -s,
                          0, s, c;
    }
    else if(axis == Eigen::Vector3f(0, 1, 0)) {
        rotationMatrix << c, 0, -s,
                          0, 1, 0,
                          s, 0, c;
    } else if(axis == Eigen::Vector3f(0, -1, 0)) {
        rotationMatrix << c, 0, s,
                          0, 1, 0,
                          -s, 0, c;
    }
    else if(axis == Eigen::Vector3f(0, 0, 1)) {
        rotationMatrix << c, s, 0,
                          -s, c, 0,
                          0, 0, 1;
    } else if(axis == Eigen::Vector3f(0, 0, -1)) {
        rotationMatrix << c, -s, 0,
                          s, c, 0,
                          0, 0, 1;
    }

    return rotationMatrix;
}
auto roundVector(Eigen::Vector3f v) {
    return Eigen::Vector3f(
        std::round(v.x()),
        std::round(v.y()),
        std::round(v.z())
    );
}
void Cube::rotatePiece(Piece& piece, Eigen::Matrix3f rotationMatrix) {
    piece.position = roundVector(rotationMatrix * piece.position);
    if(auto* corner = dynamic_cast<Corner*>(&piece)) {
        for (int i = 0; i < 3; ++i) {
            corner->tileVectors[i] = roundVector(rotationMatrix * corner->tileVectors[i]);
            corner->updateOrientation();
        }
    } else if (auto* edge = dynamic_cast<Edge*>(&piece)) {
        for (int i = 0; i < 2; ++i) {
            edge->tileVectors[i] = roundVector(rotationMatrix * edge->tileVectors[i]);
            edge->updateOrientation();
        }
    } 
}

void Cube::rotateFace(Enums::FaceEnum face, int amount) {
    //std::cout<<"Rotating Pieces"<<std::endl;

    Eigen::Vector3f desirednormal = FaceNormals.at(face);
    float angle = (float)90.0f*amount * (std::numbers::pi / 180.0f);

    Eigen::Matrix3f rotationMatrix = buildRotationMatrix(desirednormal, angle);
    
    for (size_t i = 0; i < Pieces.size(); i++) { //X
        for (size_t j = 0; j < Pieces.size(); j++) { //Y
            for(size_t k = 0; k < Pieces.size(); k++) { //Z
                if(!Pieces[i][j][k]) continue;
                Piece* p = Pieces[i][j][k].get();

                if(face == TOP && p->position.y() != maxpoint) continue;
                if(face == BOTTOM && p->position.y() != minpoint) continue;
                if(face == FRONT && p->position.z() != maxpoint) continue;
                if(face == BACK && p->position.z() != minpoint) continue;
                if(face == RIGHT && p->position.x() != maxpoint) continue;
                if(face == LEFT && p->position.x() != minpoint) continue;

                rotatePiece(*Pieces[i][j][k], rotationMatrix);
            }
        }
    }
}


float Cube::getSize() {
    return Pieces.size();
}
Piece* Cube::getPieceAtPosition(Eigen::Vector3f pos) {
    for (size_t i = 0; i < Pieces.size(); i++) { //X
        for (size_t j = 0; j < Pieces.size(); j++) { //Y
            for(size_t k = 0; k < Pieces.size(); k++) { //Z
                if(Pieces[i][j][k] && Pieces[i][j][k]->position == pos) {
                    return Pieces[i][j][k].get();
                }
            }
        }
    }
    return nullptr;
}
std::vector<std::vector<string>> Cube::getFaceColors(FaceEnum face) {
    std::vector<std::vector<string>> colors(Pieces.size(), std::vector<string>(Pieces.size(), ""));
    std::vector<string> colorlist;
    Eigen::Vector3f desirednormal = FaceNormals.at(face);

    //std::cout<<"Getting Face Colors For: "<<face<<std::endl;

    for (int i = minpoint; i <= maxpoint; i++) { //Rows
        for (int j = minpoint; j <= maxpoint; j++) { //Columns
            Eigen::Vector3f pos;
            if(face == TOP) pos = Eigen::Vector3f(j, maxpoint, i);
            if(face == BOTTOM) pos = Eigen::Vector3f(j, minpoint, -i);
            if(face == FRONT) pos = Eigen::Vector3f(j, -i, maxpoint);
            if(face == BACK) pos = Eigen::Vector3f(-j, -i, minpoint);
            if(face == RIGHT) pos = Eigen::Vector3f(maxpoint, -i, -j);
            if(face == LEFT) pos = Eigen::Vector3f(minpoint, -i, j);//REVERSED

            Piece* p = getPieceAtPosition(pos);

            int a0 = i - minpoint; //ROWS x COLUMNS
            int b0 = j - minpoint;

            if (auto* corner = dynamic_cast<Corner*>(p)) {
                for(int l = 0; l < 3; l++) {
                    if(corner->tileVectors[l] == desirednormal) {
                        colors[a0][b0] = corner->Colors[l];
                    }
                }
            } else if (auto* edge = dynamic_cast<Edge*>(p)) {
                for(int l = 0; l < 2; l++) {
                    if(edge->tileVectors[l] == desirednormal) {
                        colors[a0][b0] = edge->Colors[l];
                    }
                }
            } else if(auto* center = dynamic_cast<Center*>(p)) {
                if(center->tileVectors[0] == desirednormal) {
                    colors[a0][b0] = center->Colors[0];
                }   
            }
        }
    }
    return colors;
}

void Cube::printFace(std::vector<std::vector<string>> facedata) {
    //std::cout<<"Printing Face Data"<<std::endl;
    std::unordered_map<string, fmt::color> facetofmt = {
        {"Yellow", fmt::color::yellow},
        {"White", fmt::color::white},
        {"Green", fmt::color::green},
        {"Blue", fmt::color::blue},
        {"Orange", fmt::color::orange},
        {"Red", fmt::color::red},
    };
    for(size_t i = 0; i < facedata.size(); i++) {
        for(size_t j = 0; j < facedata[i].size(); j++) {
            string color = facedata[i][j];
            fmt::print(fmt::fg(facetofmt.at(color)), "{}{}", PIECE_CHAR, PIECE_SEPARATOR);
            if((int)(j+1) % cubesize == 0) fmt::print(FACE_SEPARATOR);
        }
        std::cout<<std::endl;
    }
}

void Cube::print() { 
    string padding(cubesize * 2 + 1, ' ');
    //std::vector<std::vector<std::vector<string>>> Faces;
    //Faces.push_back(getFaceColors(TOP));
    auto top = getFaceColors(TOP);
    for(int i = 0; i < cubesize; i++) {
        fmt::print("{}", padding);
        printFace({top[i]});
    }

    std::vector<std::vector<std::vector<string>>> middlefaces;
    middlefaces.push_back(getFaceColors(LEFT));
    middlefaces.push_back(getFaceColors(FRONT));
    middlefaces.push_back(getFaceColors(RIGHT));
    middlefaces.push_back(getFaceColors(BACK));
    
    std::vector<std::vector<string>> result(cubesize, std::vector<string>(cubesize * 4, ""));
    for(int row = 0; row < cubesize; row++) {
        for(int column = 0; column < cubesize; column++) {
            for(int face = 0; face < 4; face++) {
                result[row][face * cubesize + column] = middlefaces[face][row][column];
            }
        }
    }
    
    printFace(result);

    auto bottom = getFaceColors(BOTTOM);
    for(int i = 0; i < cubesize; i++) {
        fmt::print("{}", padding);
        printFace({bottom[i]});
    }
}
void Cube::reset() {
    for (int i = 0; i < cubesize; ++i) {
        for (int j = 0; j < cubesize; ++j) {
            for (int k = 0; k < cubesize; ++k) {
                Pieces[i][j][k] = nullptr;
            }
        }
    }
    array<float, 4> values = {(float)cubesize, (cubesize%2 == 0) ? -cubesize/2.0f : -(cubesize-1)/2.0f, (cubesize%2 == 0) ? cubesize/2.0f : (cubesize-1)/2.0f, cubesize/2.0f};
    cubesize = (int)values[0];
    minpoint = (int)values[1];
    maxpoint = (int)values[2];
    midpoint = values[3];
    for (int i = 0; i < cubesize; ++i) {
        for (int j = 0; j < cubesize; ++j) {
            for (int k = 0; k < cubesize; ++k) {
                const bool i0 = (i == 0 || i == cubesize - 1);
                const bool j0 = (j == 0 || j == cubesize - 1);
                const bool k0 = (k == 0 || k == cubesize - 1);

                Eigen::Vector3f position;
                if(cubesize % 2 == 0) {
                    position = Eigen::Vector3f(static_cast<float>(i - cubesize/2), static_cast<float>(j - cubesize/2), static_cast<float>(k - cubesize/2));
                } else if(cubesize % 2 == 1) {
                    position = Eigen::Vector3f(static_cast<float>(i - (cubesize-1)/2), static_cast<float>(j - (cubesize-1)/2), static_cast<float>(k - (cubesize-1)/2));
                }

                std::unique_ptr<pieces::Piece> piece;
                if (i0 + j0 + k0 == 3) {
                    piece = std::make_unique<Corner>(values, position, position);
                    static_cast<Corner*>(piece.get())->init();
                } else if (i0 + j0 + k0 == 2) {
                    piece = std::make_unique<Edge>(values, position, position);
                    static_cast<Edge*>(piece.get())->init();
                } else if (i0 + j0 + k0 == 1) {
                    piece = std::make_unique<Center>(values, position, position);
                    static_cast<Center*>(piece.get())->init();
                } else {
                    continue;
                }
                Pieces[i][j][k] = std::move(piece);
            }
        }
    }
}