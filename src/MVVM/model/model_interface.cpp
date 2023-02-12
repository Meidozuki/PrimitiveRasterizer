//
// Created by user on 2022/8/4.
//

#include "model_interface.hpp"

#include <string>
#include <functional>

#include "rasterizer_config.h"
#include "basic_matrix.hpp"
#include "image_io.hpp"

//#include "MVVM/viewmodel/viewmodel.hpp"

using DType = float;

VMModel::VMModel():
        Rasterizer(RasterizerConfig::height, RasterizerConfig::width)
    {
    setModel(getModelMatrix(RasterizerConfig::model_angle));
    setEyePos({0, 1, 3});
    setProjection(GetProjectionMatrix(60, 1, 0.1, 10));
}

void VMModel::moveEye(DType dx,DType dy,DType dz) {
    const auto &eye_pos = eye_pos_;
    setEyePos({eye_pos.x() + dx, eye_pos.y() + dy, eye_pos.z() + dz});
}

void VMModel::modelRotate(DType theta) {
    //TODO:整理到raster中
    static float model_angle = 20;
    model_angle += theta;
    setModel(getModelMatrix(model_angle));
}


void VMModel::saveImage() {
    const std::string filename = "temp.jpg";

    imageio::save_img(*this, filename);
}