#include <iostream>
#include <thread>
#include <windows.h>
#include "Truck/Truck.h"

int main() {
    //std::cout << "Hello, World!" << std::endl;


    //time_t  start = time(nullptr);
    //Sleep(1000);
    //time_t  duration = time(nullptr) - start;
    //std::cout << duration <<endl;


    Truck truckFollower = Truck(2);
    truckFollower.run();
    return 0;
}
