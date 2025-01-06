#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
using namespace std;

class Passenger {
public:
    string name;
    int credits;

    Passenger(string name, int credits) : name(move(name)), credits(credits) {}
};

#endif // PASSENGER_H
