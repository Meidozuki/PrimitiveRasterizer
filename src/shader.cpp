//
// Created by user on 2022/6/6.
//

#include "shader.hpp"

#include <cmath>

inline float non_negative_dot(Vector3f a,Vector3f b) {
    return std::max(0.0f, a.dot(b));
}

Array3f Shader::shadeColor(Vector3f shade_point, Vector3f surface_normal, Vector3f surface_para) {
    Array3f color = {0,0,0};

    Array3f ambient = ka_ * ambient_light_.color * ambient_light_.intensity;

    //TODO：更改正确的实现
    auto kd = surface_para.array();
    Array3f diffuse = kd * point_light_.color * point_light_.intensity *\
            non_negative_dot(point_light_.position.normalized(), surface_normal.normalized());
    Vector3f dist=shade_point-point_light_.position;
    diffuse/=dist.squaredNorm();
    color = color + ambient + diffuse;
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

//TODO:此处需要更改
        ambient_light_.intensity = intense.value();
    }
}