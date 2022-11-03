//
// Created by user on 2022/9/18.
//

#ifndef RASTERIZER_VIEWMODEL_COMMAND_HPP
#define RASTERIZER_VIEWMODEL_COMMAND_HPP

#include <iostream>
#include <tuple>
#include <string_view>

#include <Eigen/Core>

//#include "VBaoBase.hpp"
#include "VBAO/command.hpp"

class ViewModel;

using std::string_view;

class EyeMoveCommand : public vbao::TCommand<ViewModel,Eigen::Vector3f> {
private:
    string_view _status;
public:
    explicit EyeMoveCommand(ViewModel *vm): CommandType(vm), _status("invalid") {}

    void setParameter(const std::any &param) override ;

    void execute() override;
};

class ClearCommand : public vbao::TCommand<ViewModel,void> {
public:
    using CommandType ::CommandType;
    void execute() override;
};

class RotateAngleCommand : public vbao::TCommand<ViewModel,float> {
private:
    string_view _status;
public:
    explicit RotateAngleCommand(ViewModel *vm): CommandType(vm), _status("invalid") {}

    void setParameter(const std::any &param) override ;
    void execute() override;
};

#endif //RASTERIZER_VIEWMODEL_COMMAND_HPP
