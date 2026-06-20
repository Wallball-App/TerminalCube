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

    bool wide;
    bool prime;
    int coef;
    int amount;

    for(std::string move : moves) {
        if(move.empty()) continue;
        string letter = "";
        prime = false;
        wide = false;
        if(move.contains("R")) letter = "R";
        if(move.contains("U")) letter = "U";
        if(move.contains("F")) letter = "F";
        if(move.contains("L")) letter = "L";
        if(move.contains("D")) letter = "D";
        if(move.contains("B")) letter = "B";

        if(letter.empty()) continue;
        size_t find = move.find(letter);
        int pos;
        if(find != std::string::npos) {
            pos = static_cast<int>(find);
        }

        if(move.contains("'")) prime = true;

        if(move.contains("w")) wide = true;

        coef = 1;
        if(pos != 0 && std::isdigit(move[pos - 1])) {
            coef = move[pos - 1] - '0';
        }

        amount = 1;
        if(pos + 1 < move.length() && std::isdigit(move[pos + 1])) {
            amount = move[pos + 1] - '0';
        }

        Enums::FaceEnum face = Enums::MoveToFace.at(letter);
        if(coef > 1) {
            if(wide) {
                for(int i = coef; i >= 0; i--) {
                    cube.rotateLayer(face, i, amount * (prime ? -1 : 1));
                }
            } else {
                cube.rotateLayer(face, coef, amount * (prime ? -1 : 1));
            }
        } else {
            cube.rotateFace(face, amount * (prime ? -1 : 1));
        }
    }
}