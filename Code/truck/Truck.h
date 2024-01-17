#ifndef TRUCK_H
#define TRUCK_H

#pragma once
#include "../lib/TruckMetadata.h"
#include "../controller/controller.h"
//#include "../communication/CommsModule.h"
#include "../decryptor/decryptor.h"

class Truck{
public:

    TruckMetadata self;
    //CommsModule truck_comms = CommsModule(0,100);
    Decryptor truck_decryptor = Decryptor(&self);
    controller truck_controller = controller(0, &self);

    // any other sub systems

    void run();
};

#endif




