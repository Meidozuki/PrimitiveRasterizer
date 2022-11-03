//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_VIEWMODEL_HPP
#define RASTERIZER_VIEWMODEL_HPP

#include <string_view>
using std::string_view;

#include <any>
#include <functional>
#include <optional>


#include "MVVM/model/model_interface.hpp"
#include "viewmodel_command.hpp"

#include "VBAO/viewmodel.hpp"


class ViewModel final : public vbao::TViewModelBase<VMModel>{
public:
    EyeMoveCommand eye_move_cmd_;
    ClearCommand clear_cmd_;
    RotateAngleCommand rotate_cmd_;
public:
    ViewModel();

    using VMBaseType ::setModel;
    void setModel(VMModel &&model);

    void draw(const std::vector<Triangle> &triangles) {
        getModel()->draw(triangles);
    }
    void show() {
        getModel()->show();
    }

private:

    friend class ClearCommand;
    friend class EyeMoveCommand;
    friend class RotateAngleCommand;

    void moveEye(const Eigen::Vector3f &arg) {
        getModel()->moveEye(arg[0],arg[1],arg[2]);
    }

    void clear() {
        getModel()->clearBuffer(Buffers::Color | Buffers::Depth);
    }

    void rotateModel(float theta) {
        getModel()->modelRotate(theta);
    }


};


#endif //RASTERIZER_VIEWMODEL_HPP
