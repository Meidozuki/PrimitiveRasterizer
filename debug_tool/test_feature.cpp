//
// Created on 2022/6/23.
//

#include <Eigen/Core>
#include <iostream>

int main (void) {
    Eigen::Matrix4i A;
    A << 1,1,0,0\
        ,1,1,1,0\
        ,0,1,1,0\
        ,0,0,0,0;
    std::cout << "A1:\n" << A << '\n';
    std::cout << "A2:\n" << A*A << '\n';
    std::cout << "A3:\n" << A*A*A << '\n';
    std::cout << "A4:\n" << A*A*A*A << '\n';
    std::cout << "A5:\n" << A*A*A*A*A << '\n';
}