#include "Truck.h"



void Truck::run(){
    CommsModule truck_communication = CommsModule(id,100,&self);
    controller truck_controller = controller(id, &self);
    Decryptor truck_decryptor = Decryptor(&self);
    //TODO: init, initalize logic clock .done
    self.truck_logical_clock = logicalClock();

    //TODO: test use case.done
    if(test_follower_exist_controller_behaviour){
        // leader test: test if follower exist
        truck_communication.client_IDs = {4,5,6}; // fake client

    }
    else if(test_leader_exist_controller_behaviour){
        // follower test: test if leader exist
        truck_communication.client_IDs = {-2}; // fake leader
        Message m; m.setEvent(ev_stop); m.setSenderId(-2);//test if the follower receive stop event
        self.received_message.push_back(m);
    }


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

Truck::Truck(int new_id){
    id = new_id;
}





