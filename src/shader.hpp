//
// Created by user on 2022/6/6.
//

#ifndef RASTERIZER_SHADER_HPP
#define RASTERIZER_SHADER_HPP

#include <optional>

#include <Eigen/Core>

#include "illuminant.hpp"

using std::optional;
using Eigen::Vector3f;
using Eigen::Array3f;

class Shader {
public:

    Array3f ka_;
    Array3f kd_;
    Array3f ks_;
    int specular_power_;
    Vector3f eye_pos_;
    lighting::AmbientLight ambient_light_;
    lighting::PointLight  point_light_;

    Shader();

    void setEyePosition(const Vector3f &eye) { eye_pos_ = eye;}
    void setAmbient(optional<Vector3f> ka, optional<float> intense = std::nullopt);
    void setAmbientKa(const Vector3f &ka) {ka_ = ka;}
    void setDiffuseKd(const Vector3f &kd) {kd_ = kd;}

    Array3f shadeColor(Vector3f shade_point, Vector3f surface_normal, Vector3f surface_para);
};

#endif //RASTERIZER_SHADER_HPP
