#ifndef TRUCK_H
#define TRUCK_H

#pragma once
#include "../lib/TruckMetadata.h"
#include "../controller/controller.h"
#include "../communication/CommsModule.h"
#include "../decryptor/decryptor.h"

class Truck{
    TruckMetadata self;
    controller truck_controller = controller(0, &self);
    CommsModule truck_comms = CommsModule(0,100);
    Decryptor truck_decryptor = Decryptor(&self);

    void run();
};

#endif




