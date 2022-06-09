//
// Created on 2022/6/4.
//

#ifndef MAIN_CPP_SRC_VOXEL_POLYGON_HPP_
#define MAIN_CPP_SRC_VOXEL_POLYGON_HPP_

#include <vector>

#include <Eigen/Core>

#include "utils.hpp"
#include "triangle.hpp"

using Eigen::Vector3f;
using Eigen::Vector3i;

namespace voxel {
class Mesh {
public:
    static ColorType mesh_color;
    static void setMeshColor(const Vector3f &color) {mesh_color = color;}

    std::vector<Eigen::Vector3f> vertex_pos_;
    std::vector<Eigen::Vector3i> indices_;
    std::vector<Eigen::Vector3f> vertex_normal_;
    std::vector<Eigen::Vector3i> indices_vn_;

    void getTriangles(std::vector<Triangle> &tri_list);
};

class Mesh2D : public Mesh {
public:
    int alighed_axis; //TODO:修改这一部分
    float z_;

};

class Rectangle : public Mesh2D {
public:
    Rectangle();
    Rectangle(float top, float left, float bottom, float right, float z = 0);
};

class Mesh3D : public Mesh2D{
    //考虑调用矩形绘制立方体
    ;
};
class Cube : public Mesh3D {
public:
    Cube();
    Cube(float x1,float y1,float z1,float x2,float y2,float z2);
};

}


#endif //MAIN_CPP_SRC_VOXEL_POLYGON_HPP_
