//
// Created by user on 2022/8/4.
//

#include "viewmodel.hpp"

#include <iostream>
#include <memory>



ViewModel::ViewModel(): clear_cmd_(this),
                        eye_move_cmd_(this),
                        rotate_cmd_(this)
{
//    setListener(nullptr);
}


void ViewModel::setModel(VMModel &&model) {
    auto ptr = std::make_unique<VMModel>(std::move(model));
    VMBaseType ::setModel(std::move(ptr));
}

