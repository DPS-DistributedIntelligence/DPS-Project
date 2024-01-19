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
    int id;
    Truck(int new_id);

    pthread_t t_controller;
    pthread_t t_interface;
    pthread_t t_decryptor;
    pthread_t t_communication;


    TruckMetadata self;

    bool test_follower_exist_controller_behaviour = false;
    bool test_leader_exist_controller_behaviour = false;



    // any other sub systems
    void run();

};

#endif






