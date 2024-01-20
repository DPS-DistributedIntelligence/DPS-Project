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

    int truckid;
    std::cout << "Enter Follower ID: ";
    std::cin >> truckid;
    Truck truckFollower = Truck(truckid);
    truckFollower.run();
    return 0;
}
