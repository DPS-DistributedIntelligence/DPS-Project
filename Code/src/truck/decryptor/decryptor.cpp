#include "decryptor.h"

Decryptor::Decryptor(TruckMetadata* self_new_truck) {
    self_truck = self_new_truck;
}

void *Decryptor::run_thread() {
    vector<int> surroundingTruckId;

    while(true){
        for(auto i = self_truck->received_message.begin(); i != self_truck->received_message.end(); ++i){
            // follower only interest in new event and new movement from leader
            if(self_truck->role == FOLLOWER){
                if(i->getSenderId() == self_truck->truck_leader_id){
                    event event_received = i->getEvent();
                    int speed_received = i->getSpeed();
                    movementDirection direction_received = i->getDirection();
                    if (event_received != ev_any){
                        self_truck->event_handler = event_received;
                        self_truck->received_message.pop_back();
                    }else{
                        self_truck->movement_leader.push_back({direction_received, speed_received});
                    }
                }else{ // ignore message not from leader

                }

            }
            else if(self_truck->role = NOT_SET){ // gather surrounding truck information -> get the truc list from communication
                //TODO: get list of surrounding truck

            }
            else if(self_truck->role == LEADER){
                event event_received = i->getEvent();
                if (event_received != ev_any){
                    self_truck->event_handler = event_received;
                    self_truck->received_message.pop_back();
                }
            }

        }
    }
}

void *Decryptor::run(void *context) {
    return ((Decryptor *)context)->run_thread();
}



