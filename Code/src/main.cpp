#include <iostream>
#include <thread>
#include "Truck/Truck.h"
#include <windows.h>

int main() {

    int truckid;
    std::cout << "Enter New Truck ID: ";
    std::cin >> truckid;
    Truck truckFollower = Truck(truckid);
    truckFollower.run();
    return 0;
}
