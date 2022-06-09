//
// Created by user on 2022/6/6.
//

#include "shader.hpp"

#include <cmath>
#include <iostream>

inline float non_negative_dot(Vector3f a,Vector3f b) {
    return std::max(0.0f, a.dot(b));
}

Shader::Shader() {
    ka_ = Array3f::Constant(0.2);
    kd_ = Array3f::Ones();
    ks_ = Array3f::Constant(0.6);
    point_light_.light_center = {0, 1.5, 2};
    point_light_.setIntensity(10);
}

Array3f Shader::shadeColor(Vector3f shade_point, Vector3f surface_normal, Vector3f surface_para) {
    //TODO：更改为世界坐标重新计算
    Array3f color = {0,0,0};
    Array3f absorb_color = surface_para.array();

    // La = ka * Ia
    Array3f ambient = absorb_color * ka_ * ambient_light_.intensity();

    // Ld = kd * (I/r^2) * (normal dot light)
    Vector3f light_dir = point_light_.light_center - shade_point;
    float dot_result = light_dir.normalized() .dot(surface_normal.normalized()) ;
    Array3f diffuse = absorb_color * kd_ * point_light_.intensity() *\
            non_negative_dot(light_dir.normalized(), surface_normal.normalized());
    diffuse/=light_dir.squaredNorm();
    color = color + ambient + diffuse;
    //如果大于1则clip
    color = (color >= 1.0f).select(Array3f::Ones(),color);
    return color;
}

void Shader::setAmbient(optional<Vector3f> ka, optional<float> intense) {
    if (ka.has_value()) {
#if GENERAL_DEBUG_MODE
        if (ka->minCoeff() < 0 || ka->maxCoeff() > 1) {
            std::stringstream err_info;
            err_info << "Shader::setAmbient expected ka to be [0,1], but received ka: " << ka.value();
            throw std::out_of_range(err_info.str());
        }
#endif

        ka_ = std::move(ka.value());
    }

    if (intense.has_value()) {
#if GENERAL_DEBUG_MODE
        if (ka->minCoeff() < 0) {
            std::stringstream err_info;
            err_info << "Shader::setAmbient expected intensity to be positive, but received : " << intense.value();
            throw std::out_of_range(err_info.str());
        }
#endif

        ambient_light_.setIntensity(intense.value());
    }
}