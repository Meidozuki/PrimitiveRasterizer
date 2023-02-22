//
// Created by user on 2023/2/22.
//

#include "gtest/gtest.h"

#include <cmath>
#include <memory>

#include <opencv2/highgui.hpp>

#include "obj_loader.hpp"
#include "image_io.hpp"
#include "MVVM/viewmodel/viewmodel.hpp"

#ifdef GENERAL_DEBUG_MODE
#undef GENERAL_DEBUG_MODE
#endif

TEST(test3d,objloader) {

    std::vector<Triangle> triangle_list;

    ObjLoader obj_loader;
    obj_loader.loadObjFile("../cow.obj"); // may cause error

    auto& vertices=obj_loader.vertices_buf;
    for (Eigen::Vector3i &index: obj_loader.indices_buf) {
        Triangle tri;
        for (int i=0;i < 3;++i) {
            int idx=index[i]-1;
            auto vert=vertices[idx];
            tri.setVertex(i, vert);
        }
        float col=tanh(tri.vertex(0).z())/2+0.5;
        tri.setColor(0,{0,1,col});
        tri.setColor(1,{0,1,col});
        tri.setColor(2,{0,1,col});
        tri.setNormals({});
        triangle_list.push_back(std::move(tri));
    }



    ViewModel vm;
    vm.setModel(std::make_unique<VMModel>());

    vm.draw(triangle_list);
    vm.show();

    cv::waitKey(2000);
}

int main (int argc,char** argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}