//
// Created by sindr on 06.02.2019.
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

using namespace std;

bool prime(int i){
    if(i == 2){
        return true;
    }
    int mySqrt = sqrt(i) + 1;
    for(int k = 3; k < mySqrt; k++){
        if(i % k == 0){
            return false;
        }
    }
    return true;
}

void threadRunner(mutex *primeMutex, vector<int> *primes, int min, int max){
//    cout << min << endl;
//    cout << max << endl;
    for(int i = min; i < max; i = i + 2){ //Only checks odd numbers
        if(prime(i)){
            primeMutex -> lock();
            primes->push_back(i);
            primeMutex -> unlock();
        }
    }
}

vector<int> run(int threads, int min, int maxInt){
    int numThreads = threads;
    vector<thread> myThreads;

    vector<int> primes;
    mutex primeMutex;

    cout << ">> New Run" << endl;
    cout << "Threads: " << numThreads << endl;
    cout << "Min: " << min << endl;
    cout << "Max: " << maxInt << endl;

    clock_t startClock;
    double duration;

    startClock = clock();

    int start = min;
    int stop = (maxInt - min) / 4 * 2; //Halvparten av remaining for hver tråd

    for(int i = 0; i < numThreads; i++){
        int myMin = start;
        if(myMin <= 2){
            primes.push_back(2);
            myMin = 3;
        }
        else if(myMin % 2 == 0){
            myMin += 1;
        }
        int myMax = stop;
        if(myMax % 2 != 0){
            myMax += 1;
        }

//        cout << myMin << endl;
//        cout << myMax << endl;
        thread myThread ([&primeMutex, &primes, myMin, myMax]{
            threadRunner(&primeMutex, &primes, myMin, myMax);
        });
        myThreads.push_back(move(myThread));

        start = stop;
        stop = stop + (maxInt - stop)/2;
        if(i == numThreads - 2){
            stop = maxInt;
        }
        else if (stop == maxInt){
            goto shkip;
        }
    }
    shkip:;
    for(int i = 0; i < myThreads.size(); i++){
        myThreads[i].join();
    }

    duration = ( clock() - startClock ) / (double) CLOCKS_PER_SEC;

    sort(primes.begin(), primes.end());
    //cout << ">>Primes:" << endl;
//    for(int i = 0; i < primes.size(); i++){
//        std::cout << primes[i] << std::endl;
//    }
    cout<<"Prime.size(): "<< primes.size() <<'\n';

    cout<<"Time: "<< duration << 's' <<'\n';
    return primes;
}

void findPrime( // fairly naìve iterative method. For every number, check divisibility for all numbers below square root.
        unsigned long long start,
        unsigned long long stop,
        unsigned long long step,
        std::unique_ptr<std::vector<unsigned long long>>& results
){
    results = std::make_unique<std::vector<unsigned long long>>();
    for(unsigned long long cur_num = start; cur_num < stop; cur_num += step){
        bool is_prime = cur_num % 2 == 1;
        auto max = static_cast<unsigned long long int>(std::sqrt(cur_num));
        if (max % 2 == 1) ++max;
        for(unsigned long long i = 3; i <= max; i += 1){
            if(cur_num % i == 0){
                is_prime = false;
                break;
            }
        }
        if (is_prime) results->push_back(cur_num);
    }
}

int mains(int argc, char* argv[]) {
    unsigned long long start = 1;
    unsigned long long stop = 10000000; //1 mil: 0.207s
    unsigned short threadCount = 4;

    clock_t startClock;
    double duration;
    start = clock();

    switch (argc) {
        case 1:
            break;
        case 2:
            stop = static_cast<unsigned long long int>(std::stoi(argv[1]));
            break;
        case 4:
            threadCount = static_cast<unsigned short>(std::stoi(argv[3]));
            start = static_cast<unsigned long long int>(std::stoi(argv[1]));
            stop = static_cast<unsigned long long int>(std::stoi(argv[2]));
            break;
        case 3:
            start = static_cast<unsigned long long int>(std::stoll(argv[1]));
            stop = static_cast<unsigned long long int>(std::stoll(argv[2]));
            break;

        default: // > 4
            std::cerr << "Too many arguments" << std::endl;
            std::cerr << "Format: oving1 START STOP THREADS" << std::endl;
            break;
    }

    std::vector<unsigned long long> final;
    if(2 >= start){
        final.emplace_back(2);
        start = 3;
    }
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<std::vector<unsigned long long>>> results(threadCount);

    for(unsigned short i = 0; i < threadCount; ++i){
        threads.emplace_back(findPrime, start + 2*i, stop, 2*threadCount, std::ref(results[i]));
    }
    for(auto &i : threads) i.join();

    for(auto &i : results){
        final.insert(std::end(final), std::begin(*i), std::end(*i));
    }
    std::sort(final.begin(), final.end());
    duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time: " << duration << endl;
//    for(auto &i : final){
//        std::cout << i << std::endl;
//    }

    return 0;
}