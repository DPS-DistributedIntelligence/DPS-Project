
#include "controller.h"
//#include "../Lib/lib.h"


//constructor
controller::controller(int controller_id, bool anti_collision_enable){

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
    if(own_truck_metadata->role == NOT_SET){
        bool leader_exist = find_leader();
        // set role
        if (!leader_exist){
            own_truck_metadata->role = LEADER;
            return ev_be_leader;
        } else {
            own_truck_metadata->role = FOLLOWER;
            return ev_be_follower;
        }
    }
}

// leader state have internal state
event controller::state_leader(){
    current_state = leader;
    while(true){
        if (event_handler == ev_stop){
            return ev_stop;
        }
        else if(event_handler == ev_reset){
            return ev_reset;
        }
        else if(event_handler == ev_leader_found){
            return ev_leader_found;
        }
        switch(next_state_in_leader_state){
            case moving:
                event_handler = state_moving();
                break;
            case align:
                event_handler = state_align();
                break;
            case stop:
                event_handler = state_stop();
                break;
        }
    }
    return ev_any;
}


// follower state have internal state
event controller::state_follower(){
    current_state = follower;
    while(true){
        if (event_handler == ev_stop){
            return ev_stop;
        }
        else if(event_handler == ev_reset){
            return ev_reset;
        }
        else if(event_handler == ev_no_leader_found){
            return ev_no_leader_found;
        }
        switch(next_state_in_follower_state){
            case moving:
                event_handler = state_moving();
                break;
            case align:
                event_handler = state_align();
                break;
            case stop:
                event_handler = state_stop();
                break;
        }
    }
    return ev_any;
}

event controller::state_moving(){
    current_state = moving;
    if(event_handler !=  ev_stop || event_handler !=  ev_reset || event_handler !=  ev_leader_found || event_handler !=  ev_no_leader_found){
        if(own_truck_metadata->role == LEADER){
            return move_leader();
        }else{
            return move_follower();
        }
    }else{
        return event_handler;
    }
}

event controller::state_align(){
    current_state = align;
    return event_handler; // no change
}
event controller::state_stop(){
    current_state = stop;
    return ev_stop;
}
event controller::state_system_stop(){
    current_state = system_stop;
    return event_handler; // no change
}

// setters and getters
movement controller::get_current_movement() const{
    return current_movement;
}
controllerState controller::get_current_state() const{
    return current_state;
}
truckRole controller::get_truck_role() const{
    return own_truck_metadata->role;
}

void controller::set_current_movement(movementDirection new_movement_direction){
    current_movement.direction = new_movement_direction;
}
void controller::set_current_speed(int new_movement_speed){
    current_movement.speed = new_movement_speed;
}
int controller::get_leader_id() const{
    return own_truck_metadata->truck_leader_id;
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
        own_truck_metadata->truck_leader_id = leader_id;
        return true;
    }else{
        return false;
    }

}

void controller::set_logical_clock(){
    //TODO:
}

event controller::move_leader(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO:
    return event_handler;
}
event controller::move_follower(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO:
    return event_handler;
}
event controller::move_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO:
    return event_handler;
}
event controller::move_emergency_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO:
    return event_handler;
}
event controller::move_system_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO:
    return event_handler;
}

void controller::run(){

    while(true){
        next_state_computer(event_handler);
        switch(next_state){
            case initial:
                event_handler = state_initial();
                break;
            case waiting:
                event_handler = state_waiting();
                break;
            case leader:
                event_handler = state_leader();
                break;
            case follower:
                event_handler = state_follower();
                break;
            case system_stop:
                event_handler = state_system_stop();
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
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        case follower:
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        case moving:
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        case align:
            if (event_received == ev_ready){
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        case stop:
            if (event_received == ev_ready){
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



/*
 *
 */