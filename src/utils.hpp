//
// Created on 2022/6/1.
//

#ifndef MAIN_CPP_SRC_UTILS_HPP_
#define MAIN_CPP_SRC_UTILS_HPP_

#include <Eigen/Core>

inline Eigen::Vector4f Vector3to4(const Eigen::Vector3f &v, float w = 1.0f) {
    return {v.x(), v.y(), v.z(), w};
}

#endif //MAIN_CPP_SRC_UTILS_HPP_
