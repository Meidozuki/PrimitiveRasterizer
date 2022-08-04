//
// Created by user on 2022/8/4.
//

#include "model_interface.hpp"

#include <functional>

#include "rasterizer_config.h"
#include "./viewmodel.hpp"

using DType = float;

VMModel::VMModel():
    Rasterizer(RasterizerConfig::height,RasterizerConfig::width) {
}

void VMModel::registerFunctionDict(float delta) {
    auto eye_left   = [this,delta] () { this->moveEye(-delta,0,0);};
    auto eye_right  = [this,delta] () { this->moveEye( delta,0,0);};
    auto eye_up     = [this,delta] () { this->moveEye(0, delta,0);};
    auto eye_down   = [this,delta] () { this->moveEye(0,-delta,0);};

    ViewModel::registerFunc("eye_left" ,eye_left );
    ViewModel::registerFunc("eye_right",eye_right);
    ViewModel::registerFunc("eye_up"   ,eye_up   );
    ViewModel::registerFunc("eye_down" ,eye_down );
}

void VMModel::moveEye(DType dx,DType dy,DType dz) {
    const auto &eye_pos = eye_pos_;
    this->setEyePos({eye_pos.x() + dx, eye_pos.y() + dy, eye_pos.z() + dz});
}