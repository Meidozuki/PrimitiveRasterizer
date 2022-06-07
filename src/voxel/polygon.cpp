//
// Created on 2022/6/4.
//

#include "polygon.hpp"

namespace voxel {
//static member
ColorType Mesh2D::mesh_color = Vector3f(1,1,1);

void Mesh2D::getTriangles(std::vector<Triangle> &tri_list) {
    int len = indices_.size();
    for (int i=0;i < len;++i) {
        Triangle tri;
        for (int j=0;j < 3;++j) {
            int vertex_idx = indices_[i][j];
            tri.setVertex(j, vertex_pos_[vertex_idx]);
        }
        tri.setAllColors(Mesh2D::mesh_color);

        tri_list.push_back(std::move(tri));
    }
}

Rectangle::Rectangle(float top, float left, float bottom, float right,
                     float z) {
    z_=z;

    vertex_pos_.emplace_back(left,top,z_);
    vertex_pos_.emplace_back(left,bottom,z_);
    vertex_pos_.emplace_back(right,top,z_);
    vertex_pos_.emplace_back(right,bottom,z_);

    indices_.emplace_back(0,1,2);
    indices_.emplace_back(1,2,3);
}

Cube::Cube(float x1, float y1, float z1, float x2, float y2, float z2) {

    vertex_pos_.emplace_back(x1,y1,z1);
    vertex_pos_.emplace_back(x1,y1,z2);
    vertex_pos_.emplace_back(x1,y2,z1);
    vertex_pos_.emplace_back(x1,y2,z2);
    vertex_pos_.emplace_back(x2,y1,z1);
    vertex_pos_.emplace_back(x2,y1,z2);
    vertex_pos_.emplace_back(x2,y2,z1);
    vertex_pos_.emplace_back(x2,y2,z2);

    indices_.emplace_back(1,2,0);
    indices_.emplace_back(1,2,3);
    indices_.emplace_back(1,4,0);
    indices_.emplace_back(1,4,5);
    indices_.emplace_back(2,4,0);
    indices_.emplace_back(2,4,6);
    indices_.emplace_back(5,6,4);
    indices_.emplace_back(5,6,7);
    indices_.emplace_back(3,5,1);
    indices_.emplace_back(3,5,7);
    indices_.emplace_back(3,6,2);
    indices_.emplace_back(3,6,7);

}
}