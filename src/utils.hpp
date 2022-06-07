//
// Created on 2022/6/1.
//

#ifndef MAIN_CPP_SRC_UTILS_HPP_
#define MAIN_CPP_SRC_UTILS_HPP_

#include <stdexcept>

#include <Eigen/Core>

typedef Eigen::Vector3f ColorType;

inline Eigen::Vector4f Vector3to4(const Eigen::Vector3f &v, float w = 1.0f) {
    return {v.x(), v.y(), v.z(), w};
}

inline Eigen::Vector3f FloatToVector(float f) {
    return {f,f,f};
}

#endif //MAIN_CPP_SRC_UTILS_HPP_