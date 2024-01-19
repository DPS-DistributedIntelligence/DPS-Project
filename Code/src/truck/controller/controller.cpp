#include <iostream>
#include <conio.h>
#include <windows.h>
#include "controller.h"
#define WATCHDOG_TIMEOUT_SECONDS 60

//constructor
controller::controller(int new_controller_id, TruckMetadata *new_self_truck){
    id = new_controller_id;
    self_truck = new_self_truck;
    self_truck->watchdog = time(nullptr);
    //TODO: controller initialization

    //subsystem will be further developed after integration is working
    // this->antiCollisionSystem_class.set_EmergencyStop(varAnticollisionSystem);
}

void* controller::run_thread()
{
    while(true){
        std::cout << "changing high level state" << std::endl;
        next_state_computer(self_truck->event_handler); //set next state
        self_truck->event_handler = ev_any; // reset event handler
        switch(next_state){
            case initial:
                std::cout << "entering initial state" << std::endl;
                std::cout << " " << std::endl;
                self_truck->event_handler = state_initial();
                break;
            case waiting:
                std::cout << "entering waiting state" << std::endl;
                std::cout << " " << std::endl;
                self_truck->event_handler = state_waiting();
                break;
            case leader:
                std::cout << "entering leader state" << std::endl;
                std::cout << " " << std::endl;
                self_truck->event_handler = state_leader();
                break;
            case follower:
                std::cout << "entering follower state" << std::endl;
                std::cout << " " << std::endl;
                self_truck->event_handler = state_follower();
                break;
            case system_stop:
                std::cout << "entering system stop state" << std::endl;
                std::cout << " " << std::endl;
                self_truck->event_handler = state_system_stop();
                break;
            default:
                std::cout << "Default" << std::endl;
                break;
        }
    }
    return 0;
}

// states -> high level states
event controller::state_initial(){
    current_state = initial;
    if(!initialized){
        std::cout << "initializing truck controller" << std::endl;
        //TODO: initialization
        /* Start the logical clock ticks */
        self_truck->truck_logical_clock.logicalClockTick(); // initialized by the truck not controller.
    }
    return ev_ready;
}
event controller::state_waiting(){
    current_state = waiting;
    if(self_truck->role == NOT_SET){
        std::cout << "finding leader ..." << std::endl;
        bool leader_exist = find_leader();
        // set role
        if (!leader_exist){
            std::cout << "no leader found!.setting truck role as leader" << std::endl;
            self_truck->role = LEADER;
            return ev_be_leader;
        } else {
            std::cout << "leader found!.setting truck role as follower" << std::endl;
            self_truck->role = FOLLOWER;
            return ev_be_follower;
        }
    }else{
        std::cout << "the truck role is already set" << std::endl;
    }
    return ev_any;
}
event controller::state_leader(){
    // run leader internal state
    current_state = leader;
    while(true){
        if (self_truck->event_handler == ev_stop){
            std::cout << "stop signal received" << std::endl;
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            std::cout << "reset signal received" << std::endl;
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_leader_found){
            std::cout << "new leader found" << std::endl;
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
            std::cout << "stop signal received" << std::endl;
            return ev_stop;
        }
        else if(self_truck->event_handler == ev_reset){
            std::cout << "reset signal received" << std::endl;
            return ev_reset;
        }
        else if(self_truck->event_handler == ev_no_leader_found){
            std::cout << "lost leader" << std::endl;
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
    std::cout << "entering moving state" << std::endl;
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

    static movementDirection prev_direction = MOVE_FORWARD;
    static int prev_speed = 0;

    //TODO: always check for new leader.done
    bool leader_found = find_leader();
    if (leader_found){
        std::cout << "new leader found. truck role will be changed to follower" << std::endl;
        self_truck->role = FOLLOWER;
        return ev_be_follower;
    }

    //TODO: get direction and speed (input from console).done
    while(!input_given){

    }
    input_given = false;
    if((prev_direction != this->get_current_direction()) || (prev_speed != this->get_current_speed()))
    {
        self_truck->truck_logical_clock.logicalClockTick(); // initialized by the truck not controller.
        prev_direction = this->get_current_direction();
        prev_speed = this->get_current_speed();

        if(this->get_current_direction() == MOVE_EMERGENCY_STOP)
        {
            //TODO: send message.
            Message new_message = Message();
            new_message.setEvent(ev_stop);
            //send_message_to_follower(new_message);


            std::cout << "Emergency Stop. exiting from leader state" << std::endl;
            return ev_stop; // emergency stop -> will exit leader state back to stop

        }
        else
        {
            std::cout << "New Direction: " << get_movement_direction_string(this->get_current_direction()) << std::endl;
            std::cout << "New Speed: " << this->get_current_speed() << std::endl;


            self_truck->truckMovement = {this->get_current_direction(), this->get_current_speed()};

            return self_truck->event_handler;
        }
    }
    else
    {

        return self_truck->event_handler;
    }
}
event controller::move_follower(){
    // consider only one iteration ( event will be checked at every iteration by caller)
    movement new_movement;
    //TODO: receive message, encrypt message, move, print movement. done
    self_truck->movement_leader_vec_mutex.lock();
    for(auto i = self_truck->movement_leader.begin(); i != self_truck->movement_leader.end(); i++){
        new_movement = *i;
        set_current_movement(new_movement.direction);
        set_current_speed(new_movement.speed);
        self_truck->watchdog = time(nullptr); // reset watchdog
        move(new_movement);
        break; // only read the latest one
    }
    self_truck->movement_leader_vec_mutex.unlock();

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
    std::cout << "entering stop state" << std::endl;
    return ev_stop;
}
event controller::state_system_stop(){
    current_state = system_stop;
    std::cout << "the system will be reset. it take ~5 seconds" << std::endl;
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
void controller::set_current_direction(movementDirection new_movement_direction)
{
    current_movement.direction = new_movement_direction;
}
movementDirection controller::get_current_direction(void)
{
    return current_movement.direction;
}
void controller::set_current_speed(int new_movement_speed)
{
    current_movement.speed = new_movement_speed;
}
int controller::get_current_speed(void)
{
    return current_movement.speed;
}
void controller::set_current_role(truckRole varTruckRole)
{
    self_truck->role = varTruckRole;
}
truckRole controller::get_current_role()
{
    return self_truck->role;
}
void controller::set_current_movement(movementDirection new_movement_direction){
    current_movement.direction = new_movement_direction;
}

// methods
bool controller::find_leader() {
    bool leader_found = false;
    int leader_id = -1;
    int min_id = 10000;
    //TODO needs to use pthread locking and unlocking
    //self_truck->client_IDs_vec_mutex_->lock();
    for(auto i = self_truck->surrounding_truck.begin(); i != self_truck->surrounding_truck.end(); i++){
        //TODO: find id with smallest value.done
        if(i->id < self_truck->truck_id){ // find front truck
            min_id = std::min(min_id, i->id);// get the closest truck
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
    //TODO needs to use pthread locking and unlocking
    //self_truck->client_IDs_vec_mutex_->unlock();

}
event controller::move_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator. done
    std::cout << "move_stop" << std::endl;
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}
event controller::move_emergency_stop(){
    // consider only one iteration ( event will be check at every iteration by caller)
    //TODO: print indicator. done
    std::cout << "move_emergency_stop" << std::endl;
    set_current_movement(MOVE_FORWARD);
    set_current_speed(0);
    return self_truck->event_handler;
}
event controller::move(movement new_movement){
    std::cout << "new_movement-> direction: " << new_movement.direction << ", speed: "<< new_movement.speed << std::endl;
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
/*
void* controller::controller_run_thread()
{
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
                cout << "Default" << endl;
                break;
        }
    }
    return 0;
}*/
void* controller::key_board_run_thread(){
    while(true)
    {
        if (_kbhit())
        {
            char inputChar = _getch();
            if((inputChar == 'W') | (inputChar == 'w') |
               (inputChar == 'S') | (inputChar == 's') |
               (inputChar == 'A') | (inputChar == 'a') |
               (inputChar == 'D') | (inputChar == 'd') |
               (inputChar == 'E') | (inputChar == 'e') |
               (inputChar == 'B') | (inputChar == 'b'))
            {
                //pthread_mutex_lock(&mutex);
                switch (inputChar) {
                    case 'W':
                    case 'w':
                        if(get_current_speed() > cruiseDriverLimit)
                        {
                            set_current_speed(cruiseDriverLimit);
                        }
                        else
                        {
                            set_current_speed(get_current_speed() + cruiseDriverStep);
                        }
                        set_current_direction(MOVE_FORWARD);
                        break;
                    case 'S':
                    case 's':
                        if(get_current_movement().speed == 0)
                        {
                            set_current_direction(MOVE_STOP);
                        }
                        else
                        {
                            if(get_current_movement().speed < 10)
                            {
                                set_current_speed(0);
                                set_current_direction(MOVE_STOP);
                            }
                            else
                            {
                                set_current_speed(get_current_speed() - cruiseDriverStep);
                                set_current_direction(MOVE_FORWARD);
                            }
                        }
                        break;
                    case 'A':
                    case 'a':
                        set_current_direction(MOVE_LEFT);
                        break;
                    case 'D':
                    case 'd':
                        set_current_direction(MOVE_RIGHT);
                        break;
                    case 'E':
                    case 'e':
                        set_current_direction(MOVE_EMERGENCY_STOP);
                        set_current_speed(0);
                        break;
                    case 'B':
                    case 'b':
                        set_current_direction(MOVE_STOP);
                        set_current_speed(0);
                        break;
                    default:
                        break;
                }
                //pthread_mutex_unlock(&mutex);
            }
            else if((inputChar == 'P') | (inputChar == 'p'))
            {
                //TODO: break server comms
            }
            else
            {
                /*
                 * Do Nothing
                 */
            }
            input_given = true;
        }
    }
    //return nullptr;
}
void *controller::run(void* context) {
    return ((controller *)context)->run_thread();
}
void *controller::key_board_run(void* context) {
    return ((controller *)context)->key_board_run_thread();
}

void controller::send_message_to_follower(Message message) {
    message.setSenderId(self_truck->truck_id);
    //TODO needs to use pthread locking and unlocking
    //self_truck->client_IDs_vec_mutex_->lock();
    for(auto i = self_truck->surrounding_truck_IDs->begin(); i != self_truck->surrounding_truck_IDs->end(); i++){
        if(*i != self_truck->truck_id){
            message.setReceiverId(*i);
            self_truck->send_messsage_vector_guard.lock();
            self_truck->pending_send_message.push_back(message);
            self_truck->send_messsage_vector_guard.unlock();
        }
    }
    //TODO needs to use pthread locking and unlocking
    //self_truck->client_IDs_vec_mutex_->unlock();

    std::cout << "movement was sent to follower" << std::endl;

}
