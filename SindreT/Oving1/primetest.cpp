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

#include "altMain.cpp"

int main() {
    vector<int> data = {2, 3, 5, 7, 11, 13, 17, 19};

    // Tester alle primtall mellom 0 og 20
    vector<int> testres = aaa(0, 20, 4);
    for(int i = 0; i < data.size(); i++)
        assert(data[i] == testres[i]);
}