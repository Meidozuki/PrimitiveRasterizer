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

void Triangle::setNormals(const std::vector<Vector3f> &normal)  {
    int size = normal.size();
    if (size == 1) {
        normal_[0]= normal_[1] = normal_[2] = normal.at(0);
    }
    else if (size == 0) {
        Vector3f &&OA = vertex_[1] - vertex_[0];
        Vector3f &&OB = vertex_[2] - vertex_[0];
        if (OA.norm() < 1e-5 || OB.norm() < 1e-5) {
            throw std::runtime_error("compute normal on a non-triangle, maybe its vertices are not inited.");
        }
        const Vector3f &&norm = OB.cross(OA).normalized();
        normal_[0]= normal_[1] = normal_[2] = norm;
    }
    else {
        normal_[0] = normal.at(0);
        normal_[1] = normal.at(1);
        normal_[2] = normal.at(2);
    }
}