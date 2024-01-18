#ifndef TRUCK_H
#define TRUCK_H


#pragma once
#include "../lib/TruckMetadata.h"
#include "../truck/controller/controller.h"
#include "../truck/interface/interface.h"
#include "../truck/decryptor/decryptor.h"
#include "../client/CommsModule.h"
#include <pthread.h>

class Truck{
public:
    pthread_t t_controller;
    pthread_t t_interface;
    pthread_t t_decryptor;
    pthread_t t_communication;

    TruckMetadata self;
    controller truck_controller = controller(0, &self);
    CommsModule truck_communication = CommsModule(0,100,&self);


    // any other sub systems
    void run();

};

#endif






