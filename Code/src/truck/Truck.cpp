#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock
    // init
    pthread_create(&t_controller, NULL, &controller::run, &truck_controller);
    pthread_create(&t_interface, NULL, &interface::run, &truck_interface);

    //start decryptor -> new thread
    //start controller -> new thread

    //start communication -> new thread
}





