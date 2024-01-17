#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock

    // init
    truck_controller.run();
    //start decryptor -> new thread
    //start controller -> new thread
    //start communication -> new thread
}
