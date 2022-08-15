//
// Created on 2022/6/1.
//

#ifndef MAIN_CPP_SRC_UTIL_DEFINE_HPP_
#define MAIN_CPP_SRC_UTIL_DEFINE_HPP_

#include <sstream>
#include <stdexcept>

#include <Eigen/Core>

#define GENERAL_DEBUG_MODE 1
#define VERBOSE_DEBUG_MODE 1

using Vector3fCRef = const Eigen::Vector3f&;
using ColorType = Eigen::Vector3f;


inline Eigen::Vector4f Vector3to4(const Eigen::Vector3f &v, float w = 1.0f) {
    return {v.x(), v.y(), v.z(), w};
}

inline Eigen::Vector3f FloatToVector(float f) {
    return {f,f,f};
}

#endif //MAIN_CPP_SRC_UTIL_DEFINE_HPP_
