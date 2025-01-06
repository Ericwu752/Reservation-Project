#include "Vehicle.h"
#ifndef COMPACT_H
#define COMPACT_H

// Derived Compact Class
class Compact : public Vehicle {
public:
    Compact(const string& color, const string& driver)
        : Vehicle(color, "Compact", driver) {
        // Initialize seats with cost, category, and availability
        seats["front"] = Seat(5, "front");            // front seat costs 5 credits, category "front"
        seats["back_left"] = Seat(3, "back");         // back_left seat costs 3 credits, category "back"
        seats["back_right"] = Seat(3, "back");        // back_right seat costs 3 credits, category "back"
    }

    void displaySeats() override {
        std::cout << "Compact (" << color << ") seats:\n";
        for (auto& seat : seats) {
            string status = (seat.second.status == AVAILABLE) ? "Available" :
                (seat.second.status == RESERVED) ? "Occupied" : "Invalid";
            std::cout << seat.first << " (Category: " << seat.second.category << "): "
                << status << ", Cost: " << seat.second.cost << " credits\n";
        }
    }
};

#endif
