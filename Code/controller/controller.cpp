#include <iostream>
#include <windows.h>
#include "controller.h"
#define WATCHDOG_TIMEOUT_SECONDS 60
using namespace std;

//constructor
controller::controller(int new_controller_id, TruckMetadata *new_self_truck){
    id = new_controller_id;
    self_truck = new_self_truck;
    self_truck->watchdog = time(nullptr);
    //TODO: controller initialization


    //subsystem will be further developed after integration is working
    // this->antiCollisionSystem_class.set_EmergencyStop(varAnticollisionSystem);
}

// run the high level state
void controller::run(){

    while(true){
        cout << "changing high level state" << endl;
        next_state_computer(self_truck->event_handler); //set next state
        self_truck->event_handler = ev_any; // reset event handler
        switch(next_state){
            case initial:
                cout << "entering initial state" << endl;
                cout << " " << endl;
                self_truck->event_handler = state_initial();
                break;
            case waiting:
                cout << "entering waiting state" << endl;
                cout << " " << endl;
                self_truck->event_handler = state_waiting();
                break;
            case leader:
                cout << "entering leader state" << endl;
                cout << " " << endl;
                self_truck->event_handler = state_leader();
                break;
            case follower:
                cout << "entering follower state" << endl;
                cout << " " << endl;
                self_truck->event_handler = state_follower();
                break;
            case system_stop:
                cout << "entering system stop state" << endl;
                cout << " " << endl;
                self_truck->event_handler = state_system_stop();
                break;
            default:
                break;
        }
    }
}

// states -> high level states
event controller::state_initial(){
    current_state = initial;
    if(!initialized){
        cout << "initializing truck controller" << endl;
        //TODO: initialization

        self_truck->truck_logical_clock.logicalClockTick(); // initialized by the truck not controller.
    }
    return ev_ready;
}
event controller::state_waiting(){
    current_state = waiting;
    if(self_truck->role == NOT_SET){
        cout << "finding leader ..." << endl;
        bool leader_exist = find_leader();
        // set role
        if (!leader_exist){
            cout << "no leader found!.setting truck role as leader" << endl;
            self_truck->role = LEADER;
            return ev_be_leader;
        } else {
            cout << "leader found!.setting truck role as follower" << endl;
            self_truck->role = FOLLOWER;
            return ev_be_follower;
        }
    }else{
        cout << "the truck role is already set" << endl;
    }
    return ev_any;
}
event controller::state_leader(){
    // run leader internal state
    current_state = leader;
    while(true){
        if (self_truck->event_handler == ev_stop){
            cout << "stop signal received" << endl;
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            cout << "reset signal received" << endl;
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_leader_found){
            cout << "new leader found" << endl;
            return ev_leader_found;
        }
        switch(next_state_in_leader_state){
            case moving:
                self_truck->event_handler = state_moving();
                break;
            case stop:
                self_truck->event_handler = state_stop();
                break;
            default:
                self_truck->event_handler = state_moving();
                break;
        }
    }
    return ev_any;
}
event controller::state_follower(){
    current_state = follower;
    // run follower internal state
    while(true){
        if (self_truck->event_handler == ev_stop){
            cout << "stop signal received" << endl;
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            cout << "reset signal received" << endl;
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_no_leader_found){
            cout << "lost leader" << endl;
            return ev_no_leader_found;
        }
        switch(next_state_in_follower_state){
            case moving:
                self_truck->event_handler = state_moving();
                break;
            case aligning:
                self_truck->event_handler = state_align();
                break;
            case stop:
                self_truck->event_handler = state_stop();
                break;
            default:
                self_truck->event_handler = state_moving();
                break;
        }
    }
    return ev_any;
}

// states -> low level states / internal states
event controller::state_moving(){
    current_state = moving;
    cout << "entering moving state" << endl;
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
event controller::move_leader(){
    // consider only one iteration ( event will be checked at every iteration by caller)

    movementDirection new_direction= MOVE_FORWARD;
    int speed = 0 ;

    //TODO: always check for new leader.done
    bool leader_found = find_leader();
    if (leader_found){
        cout << "new leader found. truck role will be changed to follower" << endl;
        self_truck->role = FOLLOWER;
        return ev_be_follower;
    }

    //TODO: get direction and speed (input from console).done
    string new_dir;
    string new_spd;
    cout << "Direction: " << endl;
    cin >> new_dir; cout << " " << endl;

    if (new_dir == "W"){
        new_direction = MOVE_FORWARD;
    }else if (new_dir == "S"){
        new_direction = MOVE_BACK;
    }else if (new_dir == "A"){
        new_direction = MOVE_LEFT;
    }else if (new_dir == "D"){
        new_direction = MOVE_RIGHT;
    }else if (new_dir == "E"){
        cout << "Emergency Stop. exiting from leader state" << endl;
        return ev_stop; // emergency stop -> will exit leader state back to stop
    }else{
        cout << "input invalid" << endl;
        return self_truck->event_handler;
    }

    cout << "Speed: " << endl;
    cin >> new_spd; cout << " " << endl;
    speed = stoi(new_spd);
    if (speed > 210){
        cout << "input invalid" << endl;
        return self_truck->event_handler;
    }

    movement new_movement = {new_direction, speed};

    //TODO: send message.
    Message new_message = Message();
    new_message.setDirection(new_direction);
    new_message.setSpeed(speed);
    new_message.setSenderId(self_truck->truck_id);
    //set receiver new_message.setReceiverId();
    self_truck->pending_send_message.push_back(new_message);
    cout << "movement was sent to follower" << endl;
    return self_truck->event_handler;
}
event controller::move_follower(){
    // consider only one iteration ( event will be checked at every iteration by caller)
    movement new_movement;
    //TODO: receive message, encrypt message, move, print movement. done
    for(auto i = self_truck->movement_leader.begin(); i != self_truck->movement_leader.end(); i++){
        new_movement = *i;
        set_current_movement(new_movement.direction);
        set_current_speed(new_movement.speed);
        self_truck->watchdog = time(nullptr); // reset watchdog
        move(new_movement);
        break; // only read the latest one
    }

    //TODO: watchdog: check for timeout (no new message received); if true: be leader (use counter) . done
    if((time(nullptr) - self_truck->watchdog) > WATCHDOG_TIMEOUT_SECONDS){  //no new message within 1 minute
        self_truck->role = LEADER;
        return ev_be_leader;
    }

    return self_truck->event_handler;
}

event controller::state_align(){
    return self_truck->event_handler; // no change
}
event controller::state_stop(){
    cout << "entering stop state" << endl;
    return ev_stop;
}
event controller::state_system_stop(){
    current_state = system_stop;
    cout << "the system will be reset. it take ~5 seconds" << endl;
    Sleep(5000);
    return ev_reset; // no change
}

// setters and getters
movement controller::get_current_movement(){
    return current_movement;
}
controllerState controller::get_current_state(){
    return current_state;
}
void controller::set_current_movement(movementDirection new_movement_direction){
    current_movement.direction = new_movement_direction;
}
void controller::set_current_speed(int new_movement_speed){
    current_movement.speed = new_movement_speed;
}

// methods
bool controller::find_leader() {
    bool leader_found = false;
    int leader_id = -1;
    int min_id = 10000;

    for(auto i = self_truck->surrounding_truck.begin(); i != self_truck->surrounding_truck.end(); i++){
        //TODO: find id with smallest value.done
        if(i->id < self_truck->truck_id){ // find front truck
            min_id = min(min_id, i->id);// get the closest truck
            leader_found = true;
        }else{ // consider as follower

        }
    }

    if(leader_found){
        // TODO: get leader id; done
        for(auto i = self_truck->surrounding_truck.begin(); i != self_truck->surrounding_truck.end(); i++){
            if(i->id == min_id) {
                leader_id = i->id;
            }
        }
        self_truck->truck_leader_id = leader_id;
        return !(min_id == 10000 && leader_found); // return false if something went wrong
    }else{
        return false;
    }

}
event controller::move_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator. done
    cout << "move_stop" << endl;
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}
event controller::move_emergency_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator. done
    cout << "move_emergency_stop" << endl;
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}
event controller::move(movement new_movement){
    cout << "new_movement-> direction: " << new_movement.direction << ", speed: "<< new_movement.speed << endl;
    set_current_movement(new_movement.direction);
    set_current_speed(new_movement.speed);
    return self_truck->event_handler;
}

// check for next state based on current state and event
void controller::next_state_computer(event event_received){
    /*debug
    cout << "event_received: "<< event_received << endl;
    cout << "ev_stop value: "<< ev_stop << endl;
    cout << "leader value: "<< leader << endl;
    cout << "current_state value: "<< current_state << endl;
     */
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
                if(self_truck->role == LEADER){
                    next_state = leader;
                }else{
                    next_state = follower;
                }
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
            if (event_received == ev_reset){
                next_state = initial;
            }else{
                next_state = current_state;
            }
            break;
            //TODO : complete all path.done
        case aligning:
            if (event_received == ev_stop){
                next_state = system_stop;
            }else if (event_received == ev_no_leader_found) {
                next_state = leader;
            }else if (event_received == ev_leader_found) {
                next_state = follower;
            }else if (event_received == ev_reset) {
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        case moving:
            if (event_received == ev_stop){
                next_state = system_stop;
            }else if (event_received == ev_no_leader_found) {
                next_state = leader;
            }else if (event_received == ev_leader_found) {
                next_state = follower;
            }else if (event_received == ev_reset) {
                next_state = waiting;
            }else{
                next_state = current_state;
            }
            break;
        default:
            break;
    }
}

///enum controllerState {initial, waiting, leader, follower, moving, aligning, stop, system_stop};