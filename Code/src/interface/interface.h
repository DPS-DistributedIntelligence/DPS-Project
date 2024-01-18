//
// Created by Luis on 1/17/2024.
//

#ifndef CODE_INTERFACE_H
#define CODE_INTERFACE_H

#include <unistd.h>
#include <conio.h>
#include "../controller/controller.h"
#include "../lib/TruckRole.h"

#include "pthread.h"

using namespace std;

class interface {
private:
public:
    controller self_controller;
    const int cruiseDriverStep = 10;
    const int cruiseDriverLimit = 210;
    interface(controller selfController);
    ~interface();
    void run();
    //void* thread_interface(void*);
private:
    pthread_t thread_interfaceID;
};


#endif //CODE_INTERFACE_H
