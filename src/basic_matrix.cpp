//
// Created on 2022/6/4.
//

#include "basic_matrix.hpp"

#include <Eigen/Geometry>  //cross

#include "util_func.hpp"

static inline Eigen::Vector3f getDefaultCameraLookat() {
    Eigen::Vector3f v(0,0,-1);
    return std::move(v);
}

static inline Eigen::Vector3f getDefaultCameraUpDirection() {
    Eigen::Vector3f v(0,1,0);
    return std::move(v);
}

static inline Eigen::Matrix4f getViewRotate(Eigen::Vector3f lookat, Eigen::Vector3f up) {
    // newX->x, up->y, lookat->-z
    Eigen::Vector3f &&newX = lookat.cross(up);
    Eigen::Vector4f padding(0,0,0,1);
    Eigen::Matrix4f rotate;
    rotate << Vector3to4(newX,0),\
        Vector3to4(up,0),\
        Vector3to4(-lookat,0),\
        padding;

    rotate.transposeInPlace();
    return std::move(rotate);
}

Eigen::Matrix4f getViewMatrix(Eigen::Vector3f eye_pos) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
        0, 1, 0, -eye_pos[1],
        0, 0, 1, -eye_pos[2],
        0, 0, 0, 1;

    Eigen::Matrix4f rotate = getViewRotate(getDefaultCameraLookat(), getDefaultCameraUpDirection());
    view = translate * view * rotate;

    return view;
}

Eigen::Matrix4f getModelMatrix(float rotation_angle) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    float rotate_radian = rotation_angle / 180 * M_PI;

    Eigen::Matrix4f translate;
    translate << std::cos(rotate_radian), 0, -std::sin(rotate_radian), 0,
        0, 1, 0, 0,
        std::sin(rotate_radian), 0, std::cos(rotate_radian), 0,
        0, 0, 0, 1;

    model = translate * model;

    return model;
}

Eigen::Matrix4f GetProjectionMatrix(float fov_Y, float aspect_ratio,
                                    float zNear, float zFar) {
    if (zNear < 0 || zFar < 0) {
        throw std::out_of_range("zNear and zFar must be positive");
    }

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    float theta = fov_Y / 2 / 180 * M_PI;
    float y_range = zNear * std::tan(theta); //avoid duplicated /2 *2
    float x_range = aspect_ratio * y_range;
    float z_range = (zFar - zNear) / 2;

    //由于世界坐标是右手系，相机坐标是左手系
    //经过view变换后可视物体的z坐标均为负
    Eigen::Matrix4f perspective;
    perspective << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, -(zNear + zFar), -zNear * zFar,
        0, 0, -1, 0;

    // right+left == top+bottom == 0
    Eigen::Matrix4f ortho;
    ortho << 1 / x_range, 0, 0, 0,
        0, 1 / y_range, 0, 0,
        0, 0, 1 / z_range , -(zFar + zNear) / (zFar - zNear),
        0, 0, 0, 1;

    projection = ortho * perspective;

    return projection;
}