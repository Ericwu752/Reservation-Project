#include "Vehicle.h"
#ifndef PICKUP_H
#define PICKUP_H

// Derived Pickup Class
class Pickup : public Vehicle {
public:
    Pickup(const string& color, const string& driver)
        : Vehicle(color, "Pickup", driver) {
        // Initialize a single seat with cost and category
        seats["front"] = Seat(5, "front");  // front seat costs 5 credits, category "front"
    }

    void displaySeats() override {
        std::cout << "Pickup (" << color << ") seats:\n";
        for (auto& seat : seats) {
            string status = (seat.second.status == AVAILABLE) ? "Available" :
                (seat.second.status == RESERVED) ? "Occupied" : "Invalid";
            std::cout << seat.first << " (Category: " << seat.second.category << "): "
                << status << ", Cost: " << seat.second.cost << " credits\n";
        }
    }
};

#endif
