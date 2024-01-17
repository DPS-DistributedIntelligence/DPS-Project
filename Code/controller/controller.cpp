
#include "controller.h"

//constructor
controller::controller(int new_controller_id, TruckMetadata *new_own_truck_metadata){
    id = new_controller_id;
    self_truck = new_own_truck_metadata;
    //TODO: controller initialization

}

// states
event controller::state_initial(){
    current_state = initial;
    if(!initialized){
        //TODO: initialization
    }
    return ev_ready;
}
event controller::state_waiting(){
    current_state = waiting;
    if(self_truck->role == NOT_SET){
        bool leader_exist = find_leader();
        // set role
        if (!leader_exist){
            self_truck->role = LEADER;
            return ev_be_leader;
        } else {
            self_truck->role = FOLLOWER;
            return ev_be_follower;
        }
    }
}

// leader state have internal state
event controller::state_leader(){
    current_state = leader;
    while(true){
        if (self_truck->event_handler == ev_stop){
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_leader_found){
            return ev_leader_found;
        }
        switch(next_state_in_leader_state){
            case moving:
                self_truck->event_handler = state_moving();
                break;
            case align:
                self_truck->event_handler = state_align();
                break;
            case stop:
                self_truck->event_handler = state_stop();
                break;
        }
    }
    return ev_any;
}


// follower state have internal state
event controller::state_follower(){
    current_state = follower;
    while(true){
        if (self_truck->event_handler == ev_stop){
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_no_leader_found){
            return ev_no_leader_found;
        }
        switch(next_state_in_follower_state){
            case moving:
                self_truck->event_handler = state_moving();
                break;
            case align:
                self_truck->event_handler = state_align();
                break;
            case stop:
                self_truck->event_handler = state_stop();
                break;
        }
    }
    return ev_any;
}

event controller::state_moving(){
    current_state = moving;
    if(self_truck->event_handler != ev_stop || self_truck->event_handler != ev_reset || self_truck->event_handler != ev_leader_found || self_truck->event_handler != ev_no_leader_found){
        if(self_truck->role == LEADER){
            return move_leader();
        }else{
            return move_follower();
        }
    }else{
        return self_truck->event_handler;
    }
}

event controller::state_align(){
    return self_truck->event_handler; // no change
}
event controller::state_stop(){
    return ev_stop;
}
event controller::state_system_stop(){
    current_state = system_stop;

    return self_truck->event_handler; // no change
}

// setters and getters
movement controller::get_current_movement(){
    return current_movement;
}
controllerState controller::get_current_state(){
    return current_state;
}
truckRole controller::get_truck_role(){
    return self_truck->role;
}

void controller::set_current_movement(movementDirection new_movement_direction){
    current_movement.direction = new_movement_direction;
}
void controller::set_current_speed(int new_movement_speed){
    current_movement.speed = new_movement_speed;
}
int controller::get_leader_id(){
    return self_truck->truck_leader_id;
}

// methods
bool controller::find_leader() {
    bool leader_found = false;
    int leader_id = -1;
    //TODO: get location of each truck -> find front vehicle
    //~read all message, every message contain location of truck and truck id
    //~if front truck exist, set leader_found = true (try to find the most front truck)
    //~get the leader id from the truck message

    if(leader_found){
        // TODO: get leader id;
        self_truck->truck_leader_id = leader_id;
        return true;
    }else{
        return false;
    }

}

void controller::set_logical_clock(){
    //TODO:
}

event controller::move_leader(){
    // consider only one iteration ( event will be checked at every iteration by caller)

    movementDirection new_direction= MOVE_FORWARD;
    int speed = 0 ;
    //TODO: get direction and speed (input from console)

    movement new_movement = {new_direction, speed};

    //TODO: send message
    return self_truck->event_handler;
}
event controller::move_follower(){
    // consider only one iteration ( event will be checked at every iteration by caller)
    movement new_movement;
    //TODO: receive message, encrypt message, move, print movement

    if(true){  // new message
        if(true){ // normal movement
            move(new_movement);
        }else if(true){ // stop message
            move_stop();
        }else if(true){ // emergency stop message
            move_emergency_stop();
        }

    }
    return self_truck->event_handler;
}



event controller::move_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}
event controller::move_emergency_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}

event controller::move(movement new_movement){
    set_current_movement(new_movement.direction);
    set_current_speed(new_movement.speed);
    return self_truck->event_handler;
}


void controller::run(){

    while(true){
        next_state_computer(self_truck->event_handler);
        switch(next_state){
            case initial:
                self_truck->event_handler = state_initial();
                break;
            case waiting:
                self_truck->event_handler = state_waiting();
                break;
            case leader:
                self_truck->event_handler = state_leader();
                break;
            case follower:
                self_truck->event_handler = state_follower();
                break;
            case system_stop:
                self_truck->event_handler = state_system_stop();
                break;
        }
    }
}

void controller::next_state_computer(event event_received){
    switch(current_state){
        case initial:
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;

        case waiting:
            if (event_received == ev_be_leader) {
                next_state = leader;
            }else if (event_received == ev_be_follower) {
                next_state = follower;
            }else {
                next_state = current_state;
            }
            break;

        case leader:
            if (event_received == ev_stop){
                next_state = system_stop;
            }else if (event_received == ev_leader_found) {
                next_state = follower;
            }else if (event_received == ev_reset) {
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;

        case follower:
            if (event_received == ev_stop){
                next_state = system_stop;
            }else if (event_received == ev_no_leader_found) {
                next_state = leader;
            }else if (event_received == ev_reset) {
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;


        case system_stop:
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
    }
}
