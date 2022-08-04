//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_VIEWMODEL_HPP
#define RASTERIZER_VIEWMODEL_HPP

#include <string_view>
using std::string_view;

#include <any>
#include <functional>
#include <unordered_map>
#include <optional>

#include "./model_interface.hpp"

//using CommandFunc = std::function<void(std::any &&)>;
using CommandFunc = std::function<void(void)>;
using Index = std::unordered_map<std::string_view, CommandFunc>;



class ViewModel{
public:
    static Index vmFunctionDict;

    VMModel model_;

    ViewModel();

    static bool registerFunc(string_view s, const CommandFunc &func);
    static bool deleteFunc(string_view s);
    static bool triggerFunc(string_view s);


};


#endif //RASTERIZER_VIEWMODEL_HPP
