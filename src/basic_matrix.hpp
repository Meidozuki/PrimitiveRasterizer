//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_BASIC_MATRIX_HPP
#define RASTERIZER_BASIC_MATRIX_HPP

#include <cmath>
#include <Eigen/Core>
constexpr double PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
                0, 1, 0, -eye_pos[1],
                0, 0, 1, -eye_pos[2],
                0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float rotate_radian = rotation_angle / 180 * PI;

    Eigen::Matrix4f translate;
    translate << std::cos(rotate_radian), -std::sin(rotate_radian), 0, 0,
                std::sin(rotate_radian), std::cos(rotate_radian), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

    model = translate * model;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float fov_Y, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // Create the projection matrix for the given parameters.
    // Then return it.
    float arg_top = zNear * std::tan(fov_Y / 2);
    float arg_right = aspect_ratio * arg_top;

    Eigen::Matrix4f perspective;
    perspective << zNear, 0, 0, 0,
                    0, zNear, 0, 0,
                    0, 0, zNear + zFar, -zNear * zFar,
                    0, 0, 1, 0;

    // right+left == top+bottom == 0
    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, -(zNear + zFar) / 2,
                0, 0, 0, 1;

    Eigen::DiagonalMatrix<float, 4> scale;
    scale.diagonal() << (1 / arg_top), (1 / arg_right), (2 / (zFar - zNear)), 1;

    projection = scale * translate * perspective;

    return projection;
}
#endif //RASTERIZER_BASIC_MATRIX_HPP
