//
// Created by user on 2022/6/6.
//

#ifndef RASTERIZER_SHADER_HPP
#define RASTERIZER_SHADER_HPP

#include <Eigen/Core>

#include "utils.hpp"

using Eigen::Vector3f;

class Shader {
public:
    ColorType ka_;
    ColorType kd_;
    ColorType ks_;
    Vector3f eye_pos;
    float ambient_intensity;

    Shader():ambient_intensity(0.5) {
        ka_ = ColorType::Constant(0.05);
        kd_ = ColorType::Ones();
        ks_ = ColorType::Constant(0.6);
    }

    void setEyePosition(const Vector3f &eye) {eye_pos = eye;}
    void setAmbientKa(const Vector3f &ka) {ka_ = ka;}
    void setAmbientIntensity(float intense) {ambient_intensity=intense;}

    void shadeColor();
};

#endif //RASTERIZER_SHADER_HPP
