#include "Truck.h"


void* run_controller(void*)
{
    truck_controller->run();
    return nullptr;
}

void* run_interface(void*)
{
    truck_interface->run();
    return nullptr;
}
void Truck::run(){
    //TODO: init, initalize logic clock
    pthread_create(&thread_interfaceID, nullptr , run_interface, truck_interface);
    pthread_create(&thread_controller, nullptr, run_controller, truck_controller);
    // init
    //start decryptor -> new thread
    //start controller -> new thread

    //start communication -> new thread
}



