//
// Created on 2022/5/27.
//

#include <iostream>
#include "gtest/gtest.h"
#include "gtest/gtest-printers.h"

TEST(Simple_test,test01) {
    ASSERT_NE(0,1);
}

#include <complex>
#include <opencv2/highgui.hpp>

#include "image_io.hpp"
#include "voxel/polygon.hpp"
#include "MVVM/model/model_interface.hpp"

TEST(test2d,rectangle) {
    auto imshow = [](auto &model) {
        imageio::show_img(model.asRasterizer(),"rectangle-test");
    };

    VMModel_Debug model;
    model.clearBuffer(Buffers::Color | Buffers::Depth);

    voxel::TriangleMesh::setMeshColor({1,0,0});
    {
        voxel::Rectangle rect({0,0},0.5,0.25);
        model.draw2D(rect);
    }

    voxel::TriangleMesh::setMeshColor({0,0,1});
    std::complex<float> x(1.0), i(0,1.0);
    for (int _ = 0;_ < 4;++_, x*=i) {
        voxel::Rectangle rect({0.25*x.real(),0.25*x.imag()},0.2,0.2);
        model.draw2D(rect);
        imshow(model);
        cv::waitKey(100);
    }

    imshow(model);
    cv::waitKey(500);
}

TEST(test2d,circle) {
    auto imshow = [](auto &model) {
        imageio::show_img(model.asRasterizer(),"circle-test");
    };

    VMModel_Debug model(400,400);
    model.clearBuffer(Buffers::Color | Buffers::Depth);

    voxel::TriangleMesh::setMeshColor({0,0,1});
    {
        voxel::Circle circle({0, 0}, 0.5);
        model.draw2D(circle);
    }

    voxel::TriangleMesh::setMeshColor({1,0,0});
    std::complex<float> x(1.0), i(0,1.0);
    for (int _ = 0;_ < 2;++_, x*=i*i) {
        voxel::Circle circle({0.25*x.real(),0.25*x.imag()},0.25);
        model.draw2D(circle);
        imshow(model);
        cv::waitKey(100);
    }

    imshow(model);
    cv::waitKey();
}

int main (int argc,char** argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}