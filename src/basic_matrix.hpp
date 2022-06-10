//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_BASIC_MATRIX_HPP
#define RASTERIZER_BASIC_MATRIX_HPP

#include <cmath>

#include <Eigen/Core>

Eigen::Matrix4f getModelMatrix(float rotation_angle);
Eigen::Matrix4f getViewMatrix(Eigen::Vector3f eye_pos);
Eigen::Matrix4f GetProjectionMatrix(float fov_Y, float aspect_ratio,
                                    float zNear, float zFar);
#endif //RASTERIZER_BASIC_MATRIX_HPP
