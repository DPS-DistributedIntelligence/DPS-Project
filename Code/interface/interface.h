//
// Created by Luis on 1/17/2024.
//

#ifndef CODE_INTERFACE_H
#define CODE_INTERFACE_H

#include <unistd.h>
#include <conio.h>
#include "../controller/controller.h"
#include "../lib/TruckRole.h"

using namespace std;

class interface {
private:
controller self_controller;
public:
    const int cruiseDriverStep = 10;
    const int cruiseDriverLimit = 210;
    void driverInterface(void);
};


#endif //CODE_INTERFACE_H
