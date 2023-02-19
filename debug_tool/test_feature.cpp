//
// Created on 2022/6/23.
//

#include <iostream>
#include <memory>
#include <functional>

struct A{
    int x,y,z;
};

A& f(A& a) {
    a.x=0;
    return a;
}


int main (void) {
    A a{1,2,3};
    std::cout << a.x;
    f(a);

    std::function<A&(A&)> func=f;

}