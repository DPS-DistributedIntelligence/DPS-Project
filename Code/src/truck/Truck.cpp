#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock
    // init
    pthread_create(&t_controller, NULL, &controller::controller_run, &truck_controller);
    pthread_create(&t_interface, NULL, &controller::key_board_run, &truck_controller);
    pthread_create(&t_communications, NULL, &controller::communications_run, &truck_controller);

    pthread_join(t_controller, NULL);
    pthread_join(t_interface, NULL);
    pthread_join(t_communications, NULL);
    //start decryptor -> new thread
}





