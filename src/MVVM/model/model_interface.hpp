//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_MODEL_INTERFACE_HPP
#define RASTERIZER_MODEL_INTERFACE_HPP

#include "rasterizer.hpp"
#include "image_io.hpp"

#include "VBAO/model.hpp"


class VMModel : public vbao::ModelBase,
                protected Rasterizer{
    //作为渲染器对ViewModel的接口，减少ViewModel对渲染器的耦合
    //目标：1.Raster不知道Model 2.ViewModel不知道Raster

public:

    VMModel();
    VMModel(VMModel &&ano) = default;

    //debug用
//    explicit VMModel(const Rasterizer &r): raster_(r) {}

    using Rasterizer::clearBuffer;
    using Rasterizer::draw;

    void saveImage();
    void moveEye(DType dx,DType dy,DType dz);
    void modelRotate(DType theta);

    void show() {
        imageio::show_img(*this);
    }
};

#endif //RASTERIZER_MODEL_INTERFACE_HPP
