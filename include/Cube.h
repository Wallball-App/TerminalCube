#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <fmt/core.h>

using namespace std;

namespace Enums {
    enum FaceEnum {
        TOP,
        BOTTOM,
        FRONT,
        BACK,
        RIGHT,
        LEFT
    };
    inline std::unordered_map<FaceEnum, Eigen::Vector3f> FaceNormals = {
        {TOP, Eigen::Vector3f::UnitY()},
        {BOTTOM, -Eigen::Vector3f::UnitY()},
        {FRONT, Eigen::Vector3f::UnitZ()},
        {BACK, -Eigen::Vector3f::UnitZ()},
        {RIGHT, Eigen::Vector3f::UnitX()},
        {LEFT, -Eigen::Vector3f::UnitX()},
    };
    inline std::unordered_map<FaceEnum, string> FaceColors = {
        {TOP, "Yellow"},
        {BOTTOM, "White"},
        {FRONT, "Green"},
        {BACK, "Blue"},
        {RIGHT, "Orange"},
        {LEFT, "Red"},
    };
    /*inline std::unordered_map<FaceEnum, fmt::color> FaceColors = {
        {TOP, fmt::color::yellow},
        {BOTTOM, fmt::color::white},
        {FRONT, fmt::color::green},
        {BACK, fmt::color::blue},
        {RIGHT, fmt::color::orange},
        {LEFT, fmt::color::red},
    };*/
}

namespace pieces {
    struct Piece {
        Eigen::Vector3f position;
        Eigen::Vector3f ID;
        
        int size;
        int minpoint;
        int maxpoint;
        float midpoint;
        Piece(array<float, 4> cubedetails, Eigen::Vector3f piecePosition, Eigen::Vector3f pieceID) {//size, minpoint, maxpoint, midpoint
            size = (int)cubedetails[0];
            minpoint = (int)cubedetails[1];
            maxpoint = (int)cubedetails[2];
            midpoint = cubedetails[3];

            position = piecePosition;
            ID = pieceID;
        }
        Piece();
        virtual void init() {}
        virtual ~Piece() {}
    };
    struct Corner : Piece {
        
        using Piece::Piece;
        array<Eigen::Vector3f, 3> tileVectors;
        Enums::FaceEnum normal;
        array<string, 3> Colors = {"primary", "secondary", "tertiary"};

        void init() override {
            int xmult = (position.x() > 0) ? 1 : -1; //Positive if above midpoint, negative if below
            int ymult = (position.y() > 0) ? 1 : -1;
            int zmult = (position.z() > 0) ? 1 : -1;
            tileVectors = {Eigen::Vector3f(0, ymult, 0), Eigen::Vector3f(0, 0, zmult), Eigen::Vector3f(xmult, 0, 0)}; //Vector Based Prority Organization
            
            for(int i = 0; i < 3; i++) {
                for(const auto& [facenormal_instance, normal_vector] : Enums::FaceNormals) {
                    if(normal_vector == tileVectors[i]) {
                        if(i == 0) normal = facenormal_instance;
                        Colors[i] = Enums::FaceColors.at(facenormal_instance);
                    }
                }
            }
        }

	    int orientation;
       void updateOrientation() {
            if(tileVectors[0] == Eigen::Vector3f::UnitY() || tileVectors[0] == -Eigen::Vector3f::UnitY()) {
                orientation = 0;
            } else if(tileVectors[1] == Eigen::Vector3f::UnitY() || tileVectors[1] == -Eigen::Vector3f::UnitY()) {
                orientation = 1;
            } else if(tileVectors[2] == Eigen::Vector3f::UnitY() || tileVectors[2] == -Eigen::Vector3f::UnitY()) {
                orientation = 2;
            }
        };
       
    };
    struct Edge : Piece {
        using Piece::Piece;

        array<string, 2> Colors = {"primary", "secondary"};
        Eigen::Vector3f tileVectors[2] = {Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(0, 0, 0)}; //Vector Based Prority Organization
        Eigen::Vector3f primaryNormal;
        int orientation;
        void updateOrientation() {
            if(tileVectors[0] == primaryNormal) {
                orientation = 0;
            } else if(tileVectors[1] == primaryNormal) {
                orientation = 1;
            }
        };

        void init() override {
            int xmult = (position.x() > 0) ? 1 : -1;
            int ymult = (position.y() > 0) ? 1 : -1;
            int zmult = (position.z() > 0) ? 1 : -1;


            if(position.x() > minpoint && position.x() < maxpoint) { //Checks if edge is in the middle of desired side
              tileVectors[0] = Eigen::Vector3f(0, ymult, 0);
              tileVectors[1] = Eigen::Vector3f(0, 0, zmult);
            } else if(position.y() > minpoint && position.y() < maxpoint) {
                tileVectors[0] = Eigen::Vector3f(0, 0, zmult);
                tileVectors[1] = Eigen::Vector3f(xmult, 0, 0);
            } else if(position.z() > minpoint && position.z() < maxpoint) {
                tileVectors[0] = Eigen::Vector3f(0, ymult, 0);
                tileVectors[1] = Eigen::Vector3f(xmult, 0, 0);
            } else {
                    throw std::invalid_argument(std::string("Invalid position for edge piece"));
            }
            primaryNormal = tileVectors[0];


            for(int i = 0; i < 2; i++) {
                for(const auto& [facenormal_instance, normal_vector] : Enums::FaceNormals) {
                    if(normal_vector == tileVectors[i]) {
                        Colors[i] = Enums::FaceColors.at(facenormal_instance);
                    }
                }
            }
        }
    };
    struct Center : Piece {
        using Piece::Piece;

        array<string, 1> Colors = {"primary"};
        Eigen::Vector3f tileVectors[1]; //Center Position is ALWAYS a Unit Vector
        void init() override {
            if(position.y() == maxpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::TOP);
            if(position.y() == minpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::BOTTOM);
            if(position.z() == maxpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::FRONT);
            if(position.z() == minpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::BACK);
            if(position.x() == maxpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::RIGHT);
            if(position.x() == minpoint) tileVectors[0] = Enums::FaceNormals.at(Enums::FaceEnum::LEFT);
            for(const auto& [facenormal_instance, normal_vector] : Enums::FaceNormals) {
                if(normal_vector == tileVectors[0]) {
                    Colors[0] = Enums::FaceColors.at(facenormal_instance);
                }
            }
        }
    };
}

class Cube {
    public:
        Cube(int size);
        Cube();
        float getSize();
        std::vector<std::vector<std::vector<std::unique_ptr<pieces::Piece>>>> Pieces;

        void print();
        void printFace(std::vector<std::vector<string>> facedata);
        std::vector<std::vector<string>> getFaceColors(Enums::FaceEnum face);
        void rotateFace(Enums::FaceEnum face, int amount);
        void rotateLayer(Enums::FaceEnum face, int amount);

        void reset();
    private:
        Eigen::Matrix3f buildRotationMatrix(Eigen::Vector3f axis, float angle);
        void rotatePiece(pieces::Piece& piece, Eigen::Matrix3f rotationMatrix);
        pieces::Piece* getPieceAtPosition(Eigen::Vector3f pos);
};