//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_MODEL_INTERFACE_HPP
#define RASTERIZER_MODEL_INTERFACE_HPP

#include "rasterizer.hpp"

class ViewModel;

class VMModel : public Rasterizer {
    //作为渲染器对ViewModel的接口，减少ViewModel对渲染器的耦合
    //目标：1.Raster不知道Model 2.ViewModel不知道Raster

public:
//    template<typename ...Args>
//    VMModel(Args ...args):Rasterizer(args...) {}

    VMModel();

    //debug用
    explicit VMModel(const Rasterizer &r): Rasterizer(r) {}

    void registerFunctionDict(float delta);
    void saveImage();
private:
    void moveEye(DType dx,DType dy,DType dz);
    void modelRotate(DType theta);
};

#endif //RASTERIZER_MODEL_INTERFACE_HPP
