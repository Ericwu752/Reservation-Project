#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include "Vehicle.h"
#include "Pickup.h"
#include "Compact.h"
#include "Sedan.h"
#include "Reservation.h"
#include "DataHandler.h"

using namespace std;
vector<Vehicle*> vehicles; //global vehicles vector


void menu(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations) {
    while (true) {
        cout << "1. Create Reservation\n";
        cout << "2. Modify Reservation\n";
        cout << "3. Delete Reservation\n";
        cout << "4. Print Manifest (Admin Only)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            Reservation::createReservation(vehicles, credits, reservations);
            break;
        case 2:
            Reservation::modifyReservation(vehicles, credits, reservations);
            break;
        case 3:
            Reservation::deleteReservation(vehicles, credits, reservations);
            break;
        case 4:
            Reservation::printManifestToFile(vehicles, reservations);
            break;
        case 5:
            cout << "Exiting program.\n";
            
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
int main() {
    srand(static_cast<unsigned>(time(0)));  // Seed for random PIN generation

    map<string, int> credits;               // Passenger name -> seat credits
        
    vector<Reservation> reservations;       // List of reservations

    // Load data from file
    loadData("dodgeball_team.txt", credits, vehicles);

    // Run the menu loop for managing reservations and vehicles
    menu(vehicles, credits, reservations);

    

    // Clean up dynamically allocated memory for vehicles
    for (auto& vehicle : vehicles) {
        delete vehicle;
    }

    return 0;
}
