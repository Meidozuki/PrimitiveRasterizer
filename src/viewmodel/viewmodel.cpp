//
// Created by user on 2022/8/4.
//

#include "viewmodel.hpp"

#include <iostream>


Index ViewModel::vmFunctionDict;

ViewModel::ViewModel(): model_() {
    model_.registerFunctionDict(0.03);
}


//functions for vmFunctionDict
bool ViewModel::registerFunc(string_view s, const CommandFunc &func) {
    if (vmFunctionDict.find(s) != vmFunctionDict.end()) {
        std::cerr << "Function " << s << "already registered!" << std::endl;
        return false;
    }
    vmFunctionDict.try_emplace(s, func);
    return true;
}

bool ViewModel::deleteFunc(string_view s) {
    if (vmFunctionDict.find(s) == vmFunctionDict.end()) {
        std::cerr << "Try to delete function " << s << " but it does not exist!" << std::endl;
        return false;
    }
    vmFunctionDict.erase(s);
    return true;
}

bool ViewModel::triggerFunc(string_view s) {
    if (vmFunctionDict.find(s) == vmFunctionDict.end()) {
        std::cerr << "Function " << s << " does not exist!" << std::endl;
        return false;
    }
    vmFunctionDict.at(s)();
    return true;
}
