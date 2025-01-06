#ifndef RESERVATION_H
#define RESERVATION_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Vehicle.h"

using namespace std;

class Reservation {
private:
    static constexpr const char* ADMIN_PASSWORD = "admin123"; // Hardcoded admin password

    string passengerName;
    int pin;
    string vehicleType;
    string vehicleColor;
    string seat;

public:
    Reservation(const string& name, int pin, const string& type, const string& color, const string& seat)
        : passengerName(name), pin(pin), vehicleType(type), vehicleColor(color), seat(seat) {}

    string getPassengerName() const { return passengerName; }
    string getVehicleType() const { return vehicleType; }
    int getPIN() const { return pin; }
    string getVehicleColor() const { return vehicleColor; }
    string getSeat() const { return seat; }

    static int generatePIN() { return rand() % 900 + 100; }

    static void createReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations);
    static void modifyReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations);
    static void deleteReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations);
    static void printManifestToFile(const vector<Vehicle*>& vehicles, const vector<Reservation>& reservations);
    static void displayVehicleAssignments(const vector<Vehicle*>& vehicles);
};


// Implementation of Reservation-related functions
void Reservation::displayVehicleAssignments(const vector<Vehicle*>& vehicles) {
    std::cout << "----------------------------------------\n";
    std::cout << "Vehicle Type | Vehicle Color | Seats\n";
    std::cout << "----------------------------------------\n";

    for (const auto& vehicle : vehicles) {
        std::cout << std::setw(12) << vehicle->type << " | "
            << std::setw(13) << vehicle->color << " | ";

        // Access seats via the Vehicle's getter
        const auto& seats = vehicle->getSeats();
        for (const auto& seatPair : seats) {
            const auto& seatName = seatPair.first;
            const auto& seat = seatPair.second;

            // Determine seat display status
            std::string displayValue;
            if (seat.status == AVAILABLE) {
                displayValue = "(" + std::to_string(seat.cost) + ")"; // Cost for available seats
            }
            else if (seat.status == RESERVED) {
                displayValue = "(X)"; // Reserved seats
            }
            else {
                displayValue = "(-)"; // Unavailable seats
            }

            std::cout << seatName << displayValue << " "; // Print seat details
        }

        std::cout << "\n"; // End line for the current vehicle
    }

    std::cout << "----------------------------------------\n";
}

void Reservation::createReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations) {
    displayVehicleAssignments(vehicles);
    cout << "\n";
    string passengerName;
    int seatCost;
    std::cout << "Enter passenger name: ";
    cin >> passengerName;

    if (credits.find(passengerName) == credits.end()) {
        std::cout << "Passenger not found.\n";
        return;
    }

    std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";

    if (credits[passengerName] <= 0) {
        std::cout << "Insufficient credits. Reservation cannot be made.\n";
        return;
    }

    string choice;
    std::cout << "Would you like to select a seat by category or specific seat? (category/specific): ";
    cin >> choice;

    string vehicleColor, seatCategory, seatName, Type;

    if (choice == "category") {
        std::cout << "Enter seat category (e.g., front, middle, back): ";
        cin >> seatCategory;

        for (auto& vehicle : vehicles) { // Loop through vehicles in order
            //std::cout << vehicle->color << " " << vehicle->type;
            if (vehicle->reserveSeatByCategory(seatCategory, seatName, credits[passengerName])) {
                 seatCost = vehicle->getSeatCost(seatName); // Dynamically fetch the seat cost

                // Display seat cost and remaining credits
               
                std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";

                // Generate PIN and create reservation
                int pin = generatePIN();
                reservations.push_back(Reservation(passengerName, pin, vehicle->type, vehicle->color, seatName));

                // Output success message
                std::cout << "Reservation successful! Seat: " << seatName << " in "
                    << vehicle->color << " " << vehicle->type
                    << ". Your PIN is: " << pin << "\n";
                return; // Exit after successful reservation
            }
        }

        // If no available seats in the requested category
        std::cout << "No available seat in the requested category.\n";
    }

    else if (choice == "specific") {
        std::cout << "Enter vehicle type: ";
        std::cin >> Type;
        std::cout << "Enter vehicle color: ";
        std::cin >> vehicleColor;
        std::cout << "Enter seat name (front, back_middle, back): ";
        std::cin >> seatName;

        for (size_t i = 0; i < vehicles.size(); ++i) {
            if (vehicles[i]->color == vehicleColor && vehicles[i]->type == Type) {
                int seatCost = 0;

                if (seatName == "back") {
                    // Prioritize back_left first
                   seatCost = vehicles[i]->getSeatCost("back_left");
                    if (seatCost > 0 && credits[passengerName] >= seatCost) {
                        
                        if (vehicles[i]->reserveSeat("back_left", credits[passengerName])) {
                            // Successfully reserved back_left
                            int pin = generatePIN();
                            reservations.push_back(Reservation(passengerName, pin, vehicles[i]->type, vehicleColor, "back_left"));
                            

                            std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";
                            std::cout << "Reservation successful! Seat: back_left in " << vehicleColor << " " << vehicles[i]->type
                                << ". Your PIN is: " << pin << "\n";
                            return;
                        }
                    }
                    
                    else {//if(seatCost > 0 && credits[passengerName] >= seatCost)
                        seatCost = vehicles[i]->getSeatCost("back_right");
                            if (vehicles[i]->reserveSeat("back_right", credits[passengerName])) {
                                // Successfully reserved back_right
                                int pin = generatePIN();
                                reservations.push_back(Reservation(passengerName, pin, vehicles[i]->type, vehicleColor, "back_right"));
                                credits[passengerName] -= seatCost;

                                std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";
                                std::cout << "Reservation successful! Seat: back_right in " << vehicleColor << " " << vehicles[i]->type
                                    << ". Your PIN is: " << pin << "\n";
                                return;
                            }
                        
                    }

                    // If both back_left and back_right are unavailable
                    std::cout << "Both back_left and back_right seats are unavailable or insufficient credits.\n";
                    return;
                }
                else {
                    // For any other seat, check and reserve normally
                    seatCost = vehicles[i]->getSeatCost(seatName);
                    if (seatCost > 0 && credits[passengerName] >= seatCost) {
                        if (vehicles[i]->reserveSeat(seatName, credits[passengerName])) {
                            // Successfully reserved the seat
                            int pin = generatePIN();
                            reservations.push_back(Reservation(passengerName, pin, vehicles[i]->type, vehicleColor, seatName));
                            

                            std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";
                            std::cout << "Reservation successful! Seat: " << seatName << " in " << vehicleColor << " " << vehicles[i]->type
                                << ". Your PIN is: " << pin << "\n";
                            return;
                        }
                        else {
                            // Seat could not be reserved
                            std::cout << "Seat is unavailable or insufficient credits.\n";
                        }
                    }
                }
            }
        }

        // If no matching vehicle is found
        std::cout << "Vehicle with the specified color and type not found.\n";
    }


        // If no matching vehicle is found


    else {
        std::cout << "Invalid selection. Please choose 'category' or 'specific'.\n";
    }
    
}

void Reservation::modifyReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations) {
    string passengerName;
    int pin;
    std::cout << "Enter passenger name: ";
    cin >> passengerName;
    std::cout << "Enter reservation PIN: ";
    cin >> pin;
    
    for (size_t i = 0; i < reservations.size(); ++i) {
        if (reservations[i].getPassengerName() == passengerName && reservations[i].getPIN() == pin) {
            // Release the old seat and refund credits
            for (size_t j = 0; j < vehicles.size(); ++j) {
                if (vehicles[j]->color == reservations[i].getVehicleColor() &&
                    vehicles[j]->getType() == reservations[i].getVehicleType()) {
                    
                    vehicles[j]->releaseSeat(reservations[i].getSeat());
                    int seatCost = vehicles[j]->getSeatCost(reservations[i].getSeat());
                    credits[passengerName] += seatCost; // Refund credits
                    std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";
                    break;
                }
            }

            std::cout << "Please create a new reservation.\n";
            createReservation(vehicles, credits, reservations);
            return;
        }
    }

    std::cout << "Reservation not found.\n";
}

void Reservation::deleteReservation(vector<Vehicle*>& vehicles, map<string, int>& credits, vector<Reservation>& reservations) {
    string passengerName;
    int pin;
    std::cout << "Enter passenger name: ";
    cin >> passengerName;
    std::cout << "Enter reservation PIN: ";
    cin >> pin;

    // Search for the reservation by passenger name and PIN
    for (size_t i = 0; i < reservations.size(); ++i) {
        if (reservations[i].getPassengerName() == passengerName && reservations[i].getPIN() == pin) {
           // std::cout << reservations[i].getPassengerName() << " " << reservations[i].getSeat();
            // Search for the vehicle that matches the reservation color
            for (size_t j = 0; j < vehicles.size(); ++j) {
                if (vehicles[j]->color == reservations[i].getVehicleColor() &&
                    vehicles[j]->getType() == reservations[i].getVehicleType()) {
                    string seatName = reservations[i].getSeat(); // The seat name stored in the reservation

                    // Check if the seat exists in the vehicle and if it is reserved
                    //vehicles[j] -> displaySeats() ;
                    int seatCost = vehicles[j]->getSeatCost(seatName);
                    if (seatCost == -1) {
                        // If seat doesn't exist or is invalid
                        std::cout << "Seat '" << seatName << "' not found in this vehicle.\n";
                        return;
                    }

                    // If seat is reserved, release the seat and refund credits
                    vehicles[j]->releaseSeat(seatName);
                    credits[passengerName] += seatCost; // Refund credits for the seat
                    std::cout << "Passenger has " << credits[passengerName] << " seat credits remaining.\n";
                    break;
                }
            }

            // After releasing the seat, delete the reservation
            reservations.erase(reservations.begin() + i);
            std::cout << "Reservation deleted successfully.\n";
            return;
        }
    }

    // If the reservation was not found
    std::cout << "Reservation not found.\n";
}

void Reservation::printManifestToFile(const vector<Vehicle*>& vehicles, const vector<Reservation>& reservations) {
    string enteredPassword;

    // Prompt for the administrator password
    cout << "Enter the administrator password: ";
    cin >> enteredPassword;

    // Check the entered password
    if (enteredPassword != ADMIN_PASSWORD) {
        cout << "Error: Incorrect password. Returning to the main menu.\n";
        return;
    }

    // Open the file for writing
    ofstream outFile("all_reservations.txt");
    if (!outFile) {
        cout << "Error: Unable to create file.\n";
        return;
    }

    // Write the manifest to the file
    outFile << "All Reservations Manifest\n";
    outFile << "-----------------------------------------------\n";

    for (const auto& vehicle : vehicles) {
        outFile << vehicle->color << " " << vehicle->type << "\n";
        outFile << "Driver: " << vehicle->driver << "\n";

        for (const auto& seatPair : vehicle->getSeats()) {
            const string& seatName = seatPair.first;
            const Seat& seat = seatPair.second;

            if (seat.status == RESERVED) {
                // Find the passenger for the reserved seat
                for (const auto& reservation : reservations) {
                    if (reservation.getVehicleType() == vehicle->type &&
                        reservation.getVehicleColor() == vehicle->color &&
                        reservation.getSeat() == seatName) {
                        outFile << "Seat " << seatName << ": " << reservation.getPassengerName() << "\n";
                        break;
                    }
                }
            }
            else {
                outFile << "Seat " << seatName << ": Unassigned\n";
            }
        }

        outFile << "-----------------------------------------------\n";
    }

    outFile.close();
    cout << "Manifest successfully written to 'all_reservations.txt'.\n \n";
}


#endif // RESERVATION_H
