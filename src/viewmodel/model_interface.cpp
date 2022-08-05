//
// Created by user on 2022/8/4.
//

#include "model_interface.hpp"

#include <string>
#include <functional>

#include "rasterizer_config.h"
#include "basic_matrix.hpp"
#include "image_io.hpp"

#include "./viewmodel.hpp"

using DType = float;

VMModel::VMModel():
    Rasterizer(RasterizerConfig::height,RasterizerConfig::width)
    {
    setModel(getModelMatrix(RasterizerConfig::model_angle));
    setEyePos({0,1,3});
    setProjection(GetProjectionMatrix(60, 1, 0.1, 10));
}

void main_func() {
    ;
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

    constexpr int angle_delta = 10;
    auto angle_plus  = [this,angle_delta] () { this->modelRotate( angle_delta);};
    auto angle_minus = [this,angle_delta] () { this->modelRotate(-angle_delta);};
    ViewModel::registerFunc("angle_plus" ,angle_plus );
    ViewModel::registerFunc("angle_minus",angle_minus);
}

void VMModel::moveEye(DType dx,DType dy,DType dz) {
    const auto &eye_pos = eye_pos_;
    this->setEyePos({eye_pos.x() + dx, eye_pos.y() + dy, eye_pos.z() + dz});
}

void VMModel::modelRotate(DType theta) {
    //TODO:整理到raster中
    static float model_angle = 140;
    model_angle += theta;
    this->setModel(getModelMatrix(model_angle));
}


void VMModel::saveImage() {
    const std::string filename = "temp.jpg";

    imageio::save_img(*this, filename);
}