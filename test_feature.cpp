//
// Created on 2022/6/23.
//

#include <iostream>
#include <vector>
#include <array>
#include <variant>
#include <optional>
#include <Eigen/Core>
using namespace std;

void f(const std::vector<int> &v) {
    std::cout << v.size() << std::endl;
}

int main (void) {
    cout << sizeof(Eigen::Vector3f) << endl;
    cout << sizeof(array<Eigen::Vector3f,3>) << endl;
    cout << sizeof(optional<Eigen::Vector3f>) << endl;
    cout << sizeof(variant<Eigen::Vector3f, array<Eigen::Vector3f,3>, std::nullopt_t>) << endl;
    typedef variant<Eigen::Vector3f, array<Eigen::Vector3f,3> > var;
    cout << sizeof(var) << endl;
    cout << sizeof(optional<var>) << endl;
}