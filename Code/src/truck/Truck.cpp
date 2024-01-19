#include "Truck.h"

void Truck::run(){
    //TODO: init, initalize logic clock
    // init
    self.client_IDs_vec_mutex = &dummy_client_IDs_vec_mutex;

    self.send_message_vector_mutex = PTHREAD_MUTEX_INITIALIZER;
    *self.client_IDs_vec_mutex = PTHREAD_MUTEX_INITIALIZER;
    self.received_message_vector_mutex = PTHREAD_MUTEX_INITIALIZER;
    self.movement_leader_vec_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_create(&t_communication, NULL, &CommsModule::run, &truck_communication);
    pthread_create(&t_controller, NULL, &controller::run, &truck_controller);
    pthread_create(&t_interface, NULL, &controller::key_board_run, &truck_controller);
    pthread_create(&t_decryptor, NULL, &Decryptor::run, &truck_decryptor);

    pthread_join(t_controller, NULL);
    pthread_join(t_interface, NULL);
    pthread_join(t_decryptor, NULL);
    pthread_join(t_communication, NULL);
}





