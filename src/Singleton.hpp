//
// Created by user on 2023/2/2.
//

#pragma once

#include "./FP_util.hpp"

template <typename T>
Trait Singleton {
public:
    using SingletonType = T;
    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = default;
    Singleton& operator= (const Singleton&) = default;
};
