#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock
    // init
    pthread_create(&t_communication, NULL, &CommsModule::run, &truck_communication);
    pthread_create(&t_controller, NULL, &controller::run, &truck_controller);
    pthread_create(&t_interface, NULL, &controller::key_board_run, &truck_controller);
    //pthread_create(&t_decryptor, NULL, &Decryptor::run, &truck_decryptor);

    pthread_join(t_controller, NULL);
    pthread_join(t_interface, NULL);
    //pthread_join(t_decryptor, NULL);
    pthread_join(t_communication, NULL);

    //start communication -> new thread
}

Truck::Truck(int truckId) : truckID(truckId)
{

}





