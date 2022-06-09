//
// Created  on 2022/5/26.
//

#include "triangle.hpp"

Triangle::Triangle() {
    vertex_[0] = Vector3f::Zero();
    vertex_[1] = Vector3f::Zero();
    vertex_[2] = Vector3f::Zero();

    color_[0] << 1.0, 1.0, 1.0;
    color_[1] << 1.0, 1.0, 1.0;
    color_[2] << 1.0, 1.0, 1.0;

    normal_[0] = Vector3f::Zero();
    normal_[1] = Vector3f::Zero();
    normal_[2] = Vector3f::Zero();
}