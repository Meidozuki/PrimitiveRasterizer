//
// Created by user on 2022/6/6.
//

#ifndef RASTERIZER_SHADER_HPP
#define RASTERIZER_SHADER_HPP

#include <optional>

#include <Eigen/Core>

#include "utils.hpp"

using std::optional;
using Eigen::Vector3f;
using Eigen::Array3f;

class Shader {
public:
    struct Light {
        //暂时只处理方向光
        int type;
        Array3f color;
        float intensity;
        Vector3f position;

        Light(float intense=1.0): intensity(intense),color(Array3f::Ones()) {}
    };

    Array3f ka_;
    Array3f kd_;
    Array3f ks_;
    Vector3f eye_pos;
    Light ambient_light_, point_light_;

    Shader() {
        ka_ = Array3f::Constant(0.1);
        kd_ = Array3f::Ones();
        ks_ = Array3f::Constant(0.6);
        point_light_.position = {5, 5, -5};
        point_light_.intensity=50;
        eye_pos = {0,1,3};
    }

    void setEyePosition(const Vector3f &eye) {eye_pos = eye;}
    void setAmbient(optional<Vector3f> ka, optional<float> intense = std::nullopt);
    void setAmbientKa(const Vector3f &ka) {ka_ = ka;}
    void setDiffuseKd(const Vector3f &kd) {kd_ = kd;}

    Array3f shadeColor(Vector3f shade_point, Vector3f surface_normal, Vector3f surface_para);
};

#endif //RASTERIZER_SHADER_HPP
