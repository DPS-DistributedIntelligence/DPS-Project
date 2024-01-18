#ifndef TRUCK_H
#define TRUCK_H

#pragma once
#include "../lib/TruckMetadata.h"
#include "../truck/controller/controller.h"
#include "../truck/interface/interface.h"
#include "../truck/decryptor/decryptor.h"
#include <pthread.h>

class Truck{
public:
    pthread_t t_controller;
    pthread_t t_interface;
    TruckMetadata self;
    //Decryptor truck_decryptor = Decryptor(&self);
    controller truck_controller = controller(0, &self);


    // any other sub systems
    void run();

};

#endif






