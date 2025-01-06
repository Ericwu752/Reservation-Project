#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "Vehicle.h"
#include "Pickup.h"
#include "Compact.h"
#include "Sedan.h"

void loadData(const string& filename, map<string, int>& credits, vector<Vehicle*>& vehicles) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return;
    }
    vector<pair<string, string>> vehicleDetails = {
    {"Purple", "Pickup"}, {"Green", "Pickup"}, {"Black", "Pickup"},
    {"Red", "Compact"}, {"Blue", "Compact"}, {"Yellow", "Compact"},
    {"Blue", "Sedan"}, {"Black", "Sedan"}, {"Green", "Sedan"}
    };

    string line;
    for (int i = 0; i < 9; ++i) {
        // Read the driver's name
        getline(file, line);
        string driver = line;

        // Extract the hardcoded vehicle color and type
        string color = vehicleDetails[i].first;
        string type = vehicleDetails[i].second;
        if (i < 3) {
            vehicles.push_back(new Pickup(color, driver));
        }
        else if (i < 6) {
            vehicles.push_back(new Compact(color, driver));
        }
        else if (i < 9) {
            vehicles.push_back(new Sedan(color, driver));
        }

        // Create and assign the corresponding vehicle object
       /* if (type == "Pickup") vehicles.push_back(new Pickup(color, driver));
        else if (type == "Compact") vehicles.push_back(new Compact(color, driver));
        else if (type == "Sedan") vehicles.push_back(new Sedan(color, driver));*/
    }

    while (getline(file, line)) {
        string firstname;
        string lastname;
        int creditsLeft;
        stringstream ss(line);
        ss >> firstname >> lastname >> creditsLeft;
        credits[firstname] = creditsLeft;
    }
    file.close();
}

void saveData(const string& filename, const map<string, int>& credits) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return;
    }

    // Replace structured bindings
    for (map<string, int>::const_iterator it = credits.begin(); it != credits.end(); ++it) {
        file << it->first << " " << it->second << "\n";
    }

    file.close();
}

#endif
