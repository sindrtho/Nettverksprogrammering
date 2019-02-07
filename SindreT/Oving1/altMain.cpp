#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <ctime>

using namespace std;

// Calculates all primes in a desierd area and adds to a given list.
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

int main(int argc, char* argv[]) {
    int lower, higher, threadcount;

    vector<int> primeList;
    mutex primeListMutex;
    time_t start;
    time_t end;

    if(argc >= 4) {
        lower = stoi(argv[1]) >= 0 ? stoi(argv[1]) : 0;
        higher = stoi(argv[2]) >= 0 ? stoi(argv[2]) : 10000000;
        threadcount = stoi(argv[3]) >= 1 ? stoi(argv[3]) : 4;
    } else {
        lower = 11;
        higher = 13;
        threadcount = 1;
    }

    if(lower <= 2) {
        primeList.push_back(2);
        lower = 3;
    } else if(lower % 2 == 0)
        lower++;

    int stepsize = threadcount * 2;


    vector<thread> threads;


    // Calculation start.
    start = time(&start);

    for(int i = 0; i < threadcount; i++) {
        threads.emplace_back([&lower, &higher, &stepsize, i, &primeList, &primeListMutex, &threadcount] {
            primes(lower + 2*i, ++higher, stepsize, &primeList, &primeListMutex);
        });
    }

    for(auto &t : threads) {
        t.join();
    }

    // Caluclation done.


    sort(primeList.begin(), primeList.end());

    end = time(&end);

    long sec = (end - start);

    cout << "Number of threads: " << threads.size() << endl;

    for(int i : primeList) {
        cout << i << endl;
    }

    cout << "Time elapsed: " << sec << "s" << endl; // Prints the time.
    cout << "Number of primes: " << primeList.size() << endl;

    // Prints the result to file.
//    ofstream myFile;
//    myFile.open("alt.txt");
//    for(auto i : primeList) {
//        myFile << to_string(i) << endl;
//    }
//    myFile.close();

}