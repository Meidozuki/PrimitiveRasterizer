//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_POLYGON_HPP
#define RASTERIZER_POLYGON_HPP

#include <vector>

#include <Eigen/Core>

#include "util_define.hpp"
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
        virtual int sign() = 0; //定义为抽象类，暂时未找到好的函数
    };

    class Mesh2D : public Mesh {
    public:
        enum AlignedAxis {axisX,axisY,axisZ};
        AlignedAxis aligned_axis;
        float z_;

        Mesh2D(): z_(1.0) {aligned_axis = axisZ;}

        void setAlignedAxis(AlignedAxis axis) {aligned_axis = axis;}
        void getTriangles(std::vector<Triangle> &tri_list);
        int sign() override {return 2;}
    };

    class Rectangle : public Mesh2D {
    public:
        Rectangle();
        Rectangle(float top, float left, float bottom, float right, float z = 0);
    };

    class Circle : public Mesh2D {
    public:
        Circle();
        Circle(float center_x,float center_y,float radius,int edges=16,float z=0);
    };

    class Mesh3D : public Mesh2D{
        //考虑调用矩形绘制立方体
        ;
        int sign() override {return 3;}
    };

    class Cube : public Mesh3D {
    public:
        Cube();
        Cube(float x1,float y1,float z1,float x2,float y2,float z2);
    };
    using Cuboid = Cube; //长方体

    class Cone : public Mesh3D {
    public:
        //默认正多棱锥
        Eigen::Array3f tip_;
        Cone();
        Cone(int edges, const Eigen::Array3f& center, float radius = 1.0, float tip_relative = 1.0);

        void set_tip(const Eigen::Array3f& tip) {tip_ = tip;}
    };

}

#endif //RASTERIZER_POLYGON_HPP
