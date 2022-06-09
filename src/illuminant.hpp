//
// Created on 2022/6/9.
//

#ifndef MAIN_CPP_SRC_ILLUMINANT_HPP_
#define MAIN_CPP_SRC_ILLUMINANT_HPP_

#include <Eigen/Core>

namespace lighting {
class Illuminant {
protected:
    Eigen::Array3f light_color;
    float intensity_;

public:
    Illuminant(): Illuminant(1.0) {}
    Illuminant(float intense):intensity_(intense),light_color(Eigen::Array3f::Ones()) {}

    Eigen::Array3f intensity() {return light_color * intensity_;}
    void setIntensity(float intense) {intensity_ = intense;}
};

class AmbientLight : public Illuminant {
public:
};

class PointLight : public Illuminant {
public:
    Eigen::Vector3f light_center;
};

}

#endif //MAIN_CPP_SRC_ILLUMINANT_HPP_
