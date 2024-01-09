/*
 ============================================================================
 Name        : DPS-Platoon.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <iostream>

class LogicalClock {
public:
    LogicalClock() : value(0) {}

    // Increment the logical clock
    void tick() {
        value++;
    }

    // Update the logical clock based on received time
    void update(const LogicalClock& other) {
        value = std::max(value, other.value) + 1;
    }

    // Get the current logical clock value
    int getValue() const {
        return value;
    }

private:
    int value;
};

class Truck {
public:
    Truck(int id, LogicalClock& clock) : truckId(id), logicalClock(clock) {}

    // Send information to another truck
    void sendInfo(Truck& other) {
        // Increment the logical clock before sending
        logicalClock.tick();

        // Update the other truck's logical clock
        other.receiveInfo(logicalClock);
    }

    // Receive information from another truck
    void receiveInfo(const LogicalClock& senderClock) {
        // Update own logical clock based on the received time
        logicalClock.update(senderClock);

        // Do other processing as needed
    }

    // Display the truck's information including its logical clock
    void displayInfo() const {
        std::cout << "Truck " << truckId << " - Logical Clock: " << logicalClock.getValue() << std::endl;
    }

private:
    int truckId;
    LogicalClock& logicalClock;
};

int main(int argc, char *argv[]) {
    // Create logical clock
    LogicalClock globalClock;

    // Create trucks with logical clocks
    Truck truck1(1, globalClock);
    Truck truck2(2, globalClock);

    // Display initial information
    truck1.displayInfo();
    truck2.displayInfo();

    // Truck 1 sends information to Truck 2
    truck1.sendInfo(truck2);

    // Display updated information after communication
    truck1.displayInfo();
    truck2.displayInfo();

    return 0;
}
