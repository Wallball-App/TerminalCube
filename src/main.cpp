#include <Eigen/Dense>
#include <iostream>
#include <string>
#include "Cube.h"
#include "Algorithm_Reader.h"

int main() {
    int size;
    std::cout<<"Enter Cube Size: ";
    std::cin>>size;
    std::cout<<std::endl;

    Cube cube = Cube(size);
    Algorithm_Reader alg = Algorithm_Reader();
    std::string input;
    //cube.printFace(cube.getFaceColors(Enums::FaceEnum::RIGHT));
    cube.print();
    //alg.parse("R U R' U' R' F R2 U' R' U' R U R' F'", cube);
    //cube.print();
    while(true) {
        std::cout<<"Enter Algorithm: ";
        std::getline(std::cin, input);
        std::cout<<std::endl;
        if(input == "quit") break;
        alg.parse(input, cube);
        cube.print();
    }

    std::cin.get();
    
    return 0;
}