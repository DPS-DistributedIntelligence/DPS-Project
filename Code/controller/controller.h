
#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma once
#include "../Lib/lib.h"
#include "../lib/TruckMetadata.h"
#include <ctime>

enum controllerState {initial, waiting, leader, follower, moving, aligning, stop, system_stop};

class controller{
public:
    // constructors
    controller(int controller_id, TruckMetadata *new_own_truck_metadata);

    // attribute
    int id = -1;
    movement current_movement{};
    controllerState current_state = initial;
    controllerState next_state = initial;
    controllerState next_state_in_leader_state = moving;
    controllerState current_state_in_leader_state = moving;
    controllerState next_state_in_follower_state = moving;
    controllerState current_state_in_follower_state = moving;
    bool initialized = false;


    // parts
    TruckMetadata *self_truck{};

    // states
    event state_initial();
    event state_waiting();
    event state_leader();
    event state_follower();
    event state_moving();

    event state_align();
    event state_stop();
    event state_system_stop();

    // setters and getters
    movement get_current_movement();
    controllerState get_current_state();

    void set_current_movement(movementDirection new_movement_direction);
    void set_current_speed(int new_movement_speed);

    // methods
    void run();
    bool find_leader();
    event move_leader();
    event move_follower();
    event move_stop();
    event move_emergency_stop();
    event move(movement new_movement);

    void next_state_computer(event handler);

    // variable that already moved to the truck
    //movement_str *movement_st; // expected next movement
    //vector<controllerSystem_str> vehicleList_vector;
};

#endif
