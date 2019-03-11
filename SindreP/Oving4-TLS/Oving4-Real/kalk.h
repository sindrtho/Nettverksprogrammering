//
// Created by sindrtho on 3/1/19.
//

#ifndef OVING3_KALKULATOR_H
#define OVING3_KALKULATOR_H

#include <bits/exception.h>
#include <string>

using namespace std;

struct InvalidOperatorException: std::exception {
    const char * what() const throw() {
        return "Invalid operator";
    }
};

struct InvalidNumberException: std::exception {
    const char * what() const throw() {
        return "Invalid number";
    }
};

class kalk {
private:
    virtual int adder(int a, int b) = 0;
    virtual int substract(int a, int c) = 0;

public:
    virtual int calculate(string a, string b, char oprator) = 0;
};

#endif //OVING3_KALKULATOR_H
