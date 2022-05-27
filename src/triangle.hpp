//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_TRIANGLE_HPP
#define RASTERIZER_TRIANGLE_HPP

#include <array>

#include <Eigen/Eigen>

using std::array;
using Eigen::Vector3f;

class Triangle {

public:
    array<Vector3f ,3> vertex_;
    array<Vector3f ,3> color_;

};

#endif //RASTERIZER_TRIANGLE_HPP
