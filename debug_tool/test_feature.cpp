//
// Created on 2022/6/23.
//

#include <iostream>
#include <any>
using std::cout,std::endl;

class A{
public:
    int apub;
    void fapub() {}
protected:
    int apro;
    void fapro() {}
private:
    int apri;
    void fapri() {}
    virtual void f() =0;
};

class B: protected A {
public:
    int apub;
};

int main () {
    cout << __FILE__ << "at line " << __LINE__ << endl;
}