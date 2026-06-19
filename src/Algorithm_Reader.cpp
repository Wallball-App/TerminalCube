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
    string letter;

    for(std::string move : moves) {
        if(move.contains("R")) letter = "R";
        if(move.contains("U")) letter = "U";
        if(move.contains("F")) letter = "F";
        if(move.contains("L")) letter = "L";
        if(move.contains("D")) letter = "D";
        if(move.contains("B")) letter = "B";

        size_t find = move.find(letter);
        int pos;
        if(find != std::string::npos) {
            pos = find;
        } else {
            throw std::runtime_error("No Move Found in Term");
        }

        if(move.contains("'")) prime = true;

        if(move.contains("w")) wide = true;

        if(pos != 0) coef = move[pos-1];
        else coef = 1;

        amount = move[pos+1];

        
    }
}