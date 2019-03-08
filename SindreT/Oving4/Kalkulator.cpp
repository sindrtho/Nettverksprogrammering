#include "kalk.h"
#include <string>
#include <stdexcept>

using namespace std;

class Kalkulator: public kalk {
private:
    Kalkulator() {}
//    Kalkulator(const Kalkulator&);
//    Kalkulator& operator=(const Kalkulator&);

    int adder(int a, int b) {
        return a + b;
    }

    int substract(int a, int b) {
        return a - b;
    }

public:

    static Kalkulator &getInstance() {
        static Kalkulator instance;

        return instance;
    }

    int calculate(string a, string b, char oprator) {
        try {
            int x = stoi(a);
            int y = stoi(b);
            if(oprator == '+')
                return adder(x, y);
            else if(oprator == '-')
                return substract(x, y);
            else
                throw InvalidOperatorException();
        } catch (invalid_argument& e) {
            throw InvalidNumberException();
        }
    }
};