//
// Created by sindrtho on 2/8/19.
//

#ifndef OVING1_FINDPRIMES_H
#define OVING1_FINDPRIMES_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <ctime>
#include <cassert>

using namespace std;

void primes(int lowerLimit, int higherLimit, int stepSize, vector<int> *table, mutex *primeMut) {
    for(int i = lowerLimit; i < higherLimit; i+=stepSize) {
//        if(i != 2) {    // 2 is prime.
        for(int j = 3; j < sqrt(i) + 1; j+=2) {
            if(i % j == 0) {
                goto skip;
            }
        }
//        }
        primeMut->lock();
        table->push_back(i);
        primeMut->unlock();
        skip:;
    }
}

#endif //OVING1_FINDPRIMES_H
