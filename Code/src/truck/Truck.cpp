#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock
    // init
    pthread_create(&t_controller, NULL, &controller::run, &truck_controller);
    pthread_create(&t_interface, NULL, &controller::key_board_run, &truck_controller);

    pthread_join(t_controller, NULL);
    pthread_join(t_interface, NULL);
    //start decryptor -> new thread
    //start controller -> new thread

    //start communication -> new thread
}





