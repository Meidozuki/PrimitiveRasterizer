//
// Created by user on 2022/9/18.
//

#include "viewmodel_command.hpp"

#include "viewmodel.hpp"

void EyeMoveCommand::setParameter(const std::any &param) {
    auto pos = std::any_cast<string_view>(param);

    float delta=0.03;
    if (pos == "left") {
        _status = pos;
        arg_ = {-delta, 0, 0};
    }
    else if (pos == "right") {
        _status = pos;
        arg_ = {delta, 0, 0};
    }
    else if (pos == "up") {
        _status = pos;
        arg_ = {0, delta, 0};
    }
    else if (pos == "down") {
        _status = pos;
        arg_ = {0, -delta, 0};
    }
    else {
        _status = "invalid";
    }
}

void EyeMoveCommand::execute()  {
    if (_status == "invalid") {
        std::cerr << "Invalid Argument! at file" << __FILE__ << " line " << __LINE__ << std::endl;
    }
    else {
        pVM_->moveEye(arg_);
    }
}

void RotateAngleCommand::setParameter(const std::any &param) {
    auto pos = std::any_cast<string_view>(param);

    float delta=10;
    if (pos == "plus") {
        _status = pos;
        arg_ = delta;
    }
    else if (pos == "minus") {
        _status = pos;
        arg_ = -delta;
    }
    else {
        _status = "invalid";
    }
}

void RotateAngleCommand::execute() {
    if (_status == "invalid") {
        std::cerr << "Invalid Argument! at file" << __FILE__ << " line " << __LINE__ << std::endl;
    }
    else {
        pVM_->rotateModel(arg_);
    }
}


void ClearCommand::execute() {
    pVM_->clear();
}

