#include "decryptor.h"

Decryptor::Decryptor(TruckMetadata* self_new_truck) {
    self_truck = self_new_truck;
}

void Decryptor::run() {
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
                int new_truck_id = i->getSenderId();
                int new_truck_x = i->getLocation().x;
                bool exist = false;

                for(auto i = surroundingTruckId.begin(); i != surroundingTruckId.end(); i++){
                    if (new_truck_id == *i){
                        exist = true;
                    }
                }
                if(!exist){
                    surroundingTruckId.push_back(new_truck_id);
                    self_truck->surrounding_truck.push_back({new_truck_x,new_truck_id});
                }

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