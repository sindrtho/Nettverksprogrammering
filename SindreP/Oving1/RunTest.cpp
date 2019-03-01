//
// Created by sindr on 08.02.2019.
//

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <functional>
#include <cassert>
#include "main.cpp"

using namespace std;

int main(){
    cout << "Oving1: Threads og Primes!" << endl;
    //tests
//    int test1 = run(2, 1, 1000);
//    int test2 = run(4, 1, 10000);
//    int test3 = run(8, 10000, 100000);
      run(8, 1, 10000000);
//    cout << "Test 1: " << (test1 == 168) << endl;
//    cout << "Test 2: " << (test2 == 1230) << endl;
//    cout << "Test 3: " << (test3 == 8363) << endl;

    vector<int> expected = {2, 3, 5, 7, 11, 13, 17, 19};
    vector<int> res = run(2, 1, 20);
    cout << ">>Testing: " << endl;
    for(int i = 0; i < res.size(); i++){
        assert(res[i] == expected[i]);
    }
    cout << "Tests done " << endl;

    return 0;
}