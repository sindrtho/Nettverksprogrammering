//
// Created by sindr on 06.02.2019.
//

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <thread>

using namespace std;

bool prime(int i){
    if(i == 2){
        return true;
    }
    for(int k = 2; k < sqrt(i); k++){
        if(i % k == 0){
            return false;
        }
    }
    return true;
}

void threadRunner(vector<int> *primes, int min, int max){
    for(int i = min; i < max; i = i + 2){ //Only checks odd numbers
        if(prime(i)){
            primes->push_back(i);
        }
    }
}

int main(){
    std::cout << "Oving1: Threads og Primes!" << std::endl;

    int numThreads = 1;
    int maxInt = 1000;
    vector<int> primes;

    int part = maxInt / numThreads;
    int min = 1;
    for(int i = 0; i < numThreads; i++){

    }
    thread myThread ([&primes, &min, &maxInt]{
        threadRunner(&primes, min, maxInt);
    });

    myThread.join();

    for(int i = 0; i < primes.size(); i++){
        std::cout << primes[i] << std::endl;
    }
    return 0;
}