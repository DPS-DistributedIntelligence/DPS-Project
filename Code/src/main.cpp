#include <iostream>
#include <thread>
#include <windows.h>
#include "Truck/Truck.h"

int main() {
    int mainTruckID;

    std::cout << "Enter truck ID... ";
    std::cin >> mainTruckID;

    Truck truck1 = Truck(mainTruckID);
    truck1.run();
    return 0;
}
