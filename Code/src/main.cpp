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


    Truck truck1 = Truck();
    truck1.run();
    return 0;
}
