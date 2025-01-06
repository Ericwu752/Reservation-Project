#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std; 
enum SeatStatus { AVAILABLE, RESERVED, INVALID }; // Enumerated type for the seats

struct Seat {
    int cost;            // Seat cost in credits
    SeatStatus status;   // Seat status (Available, Reserved, Invalid)
    string category;     // Seat category (e.g., "front", "middle", "back")

    // Constructor to initialize the seat with a cost, category, and default status (AVAILABLE)
    Seat(int costValue, const string& seatCategory)
        : cost(costValue), status(AVAILABLE), category(seatCategory) {}

    // Default constructor for cases where you may need a default seat
    Seat() : cost(0), status(INVALID), category("") {}  // Default seat, not available and with 0 cost
};

class Vehicle {
protected:
    map<string, Seat> seats;  // seat name -> Seat object (which contains cost and status)
public:
    string color; // Color of vehicle
    string type;  // Type of vehicle
    string driver;// Driver of vehicle
    string getType() const { return type; }
    Vehicle(const string& color, const string& type, const string& driver)
        : color(color), type(type), driver(driver) {}

    virtual void displaySeats() = 0; // Pure virtual function for displaying seats

    const map<string, Seat>& getSeats() const {
        return seats;
    }

    bool reserveSeat(const string& seat, int& credits) {
        auto it = seats.find(seat);
        if (it != seats.end() && it->second.status == AVAILABLE) {
            if (credits >= it->second.cost) {
                credits -= it->second.cost;
                it->second.status = RESERVED;  // Mark seat as reserved
                return true;
            }
            else {
                std::cout << "Not enough credits for this seat.\n";
                return false;
            }
        }
        else {
            std::cout << "Seat not available.\n";
        }
        return false;
    }

    bool reserveSeatByCategory(const string& category, string& assignedSeat, int& credits) {
        // First check the category for each vehicle
        for (auto& seatPair : seats) {
            // Check for the "front" category
            if (category == "front" && seatPair.second.category == "front" && seatPair.second.status == AVAILABLE) {
                if (credits >= seatPair.second.cost) {
                    credits -= seatPair.second.cost;  // Deduct credits
                    seatPair.second.status = RESERVED;  // Mark seat as reserved
                    assignedSeat = seatPair.first;  // Assign this seat
                    std::cout << "Reserved " << seatPair.first << " in the front of this vehicle (" << type << " - " << color << ").\n";
                    return true;
                }
                else {
                    std::cout << "Not enough credits for the front seat.\n";
                    return false;
                }
            }

            // Check for the "middle" category
            if (category == "middle" && seatPair.second.category == "middle" && seatPair.second.status == AVAILABLE) {
                if (credits >= seatPair.second.cost) {
                    credits -= seatPair.second.cost;  // Deduct credits
                    seatPair.second.status = RESERVED;  // Mark seat as reserved
                    assignedSeat = seatPair.first;  // Assign this seat
                    std::cout << "Reserved " << seatPair.first << " in the middle of this vehicle (" << type << " - " << color << ").\n";
                    return true;
                }
                else {
                    std::cout << "Not enough credits for the middle seat.\n";
                    return false;
                }
            }

            // Check for the "back" category (prioritize back_left first, then back_right)
            if (category == "back") {
                // Check the back_left seat first
                if (seatPair.first == "back_left" && seatPair.second.status == AVAILABLE) {
                    if (credits >= seatPair.second.cost) {
                        credits -= seatPair.second.cost;  // Deduct credits
                        seatPair.second.status = RESERVED;  // Mark seat as reserved
                        assignedSeat = seatPair.first;  // Assign this seat
                        std::cout << "Reserved " << seatPair.first << " in the back left of this vehicle (" << type << " - " << color << ").\n";
                        return true;
                    }
                    else {
                        std::cout << "Not enough credits for the back left seat.\n";
                        return false;
                    }
                }
                // If back_left is not available, try back_right
                else if (seatPair.first == "back_right" && seatPair.second.status == AVAILABLE) {
                    if (credits >= seatPair.second.cost) {
                        credits -= seatPair.second.cost;  // Deduct credits
                        seatPair.second.status = RESERVED;  // Mark seat as reserved
                        assignedSeat = seatPair.first;  // Assign this seat
                        std::cout << "Reserved " << seatPair.first << " in the back right of this vehicle (" << type << " - " << color << ").\n";
                        return true;
                    }
                    else {
                        std::cout << "Not enough credits for the back right seat.\n";
                        return false;
                    }
                }
            }
        }

        // If we reach here, no available seat was found in the requested category
        std::cout << "No available seat in the '" << category << "' category in this vehicle.\n";
        return false;
    }


    void releaseSeat(const string& seat) {
        if (seats.find(seat) != seats.end() && seats[seat].status == RESERVED) {
            seats[seat].status = AVAILABLE;  // Mark seat as available again
        }
    }

    int getSeatCost(const string& seat) const {
        auto it = seats.find(seat);
        //std::cout << it->first;
        if (it != seats.end()) {
            return it->second.cost;  // Always return the seat cost, regardless of its status
        }
        std::cout << "Seat '" << seat << "' not found.\n";
        return -1;  // Indicate invalid seat if not found
    }
};

#endif
