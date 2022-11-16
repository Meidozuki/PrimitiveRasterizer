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

protected:
    Mesh() = default; //访问控制，不允许直接初始化

    std::vector<Eigen::Vector3f> vertex_pos_;
    std::vector<Eigen::Vector3i> indices_;
    std::vector<Eigen::Vector3f> vertex_normal_;
    std::vector<Eigen::Vector3i> indices_vn_;

    virtual int sign() = 0; //定义为抽象类，暂时未找到好的函数
    void getTriangles(std::vector<Triangle> &tri_list);
private:
    //定义接口，实现静多态
};
//static member
//需要C++17
inline ColorType Mesh::mesh_color = Vector3f(1,1,1);


//---Mesh2D
class Mesh2D : protected Mesh {
public:
    using Mesh::vertex_pos_, Mesh::indices_, Mesh::vertex_normal_, Mesh::indices_vn_;

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

/**
 * @brief HollowMesh意为绘制Mesh3D时提供顶点等快捷方法，最终运行性能视情况而定
 *      HollowMesh同时为一层wrapper，提供面向3D的接口
 */
class HollowMesh {};

class HollowCircle : public Circle, HollowMesh {
public:
    Eigen::Array3f center_;
    float radius_;
    int edges_;

    HollowCircle() = default;
    HollowCircle(Vector3f center,float radius,int edges=16):
        Circle(center.x(),center.y(),radius,edges,center.z()),radius_(radius),edges_(edges) {
        center_={center.x(),center.z(),center.y()};
    }
};


//---Mesh3D
class Mesh3D : public Mesh{
public:
    using Mesh::getTriangles;
    using Mesh::vertex_pos_, Mesh::indices_, Mesh::vertex_normal_, Mesh::indices_vn_;
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
    Cone();
    Cone(const Eigen::Array3f &center, int edges, float radius = 1.0, float tip_relative = 1.0);
    Cone(const HollowCircle &circle, float tip_relative = 1.0, bool has_bottom = true);

private:
    float axis_coef_;
    void face_init(int edges, bool has_bottom = true);
};

class Frustum : public Mesh3D {
public:
    Frustum();
    Frustum(const HollowCircle &lower,const HollowCircle &upper);
};

class Sphere : public Mesh3D {
public:
    Sphere();
    Sphere(const Eigen::Array3f &center, float radius, int edges=64, int parallel=20);
};

}

#endif //RASTERIZER_POLYGON_HPP
