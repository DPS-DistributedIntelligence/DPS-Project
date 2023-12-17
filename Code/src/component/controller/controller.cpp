/*
 * controller.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */


#include "controller.h"
#include "src/lib/global_variable.h"
#include <ctime>
#include <iostream>


using namespace std;
//controller::controller(int truck_id, currentGPS_st varGPS, uint32_t varTimestamp, Truck* (*getNearbyTruck)(), Location (*getSelfLocation)(), void  (*connectToLeader)(int leaderId), void  (*openNewChannel)(int truck_id))

controller::controller(int* truck_id,int* leader_id, Location *location, communication *comComponent, currentGPS_st varGPS,  uint32_t varTimestamp) {
	l_currentGPS_st.latitudeGPS_float = varGPS.latitudeGPS_float;
	l_currentGPS_st.longitudeGPS_float = varGPS.longitudeGPS_float;
    this->location = location;
    this->truck_id = truck_id;
    this->comComponent = comComponent;
    this->leader_id = leader_id;

	timestamp_u32 = varTimestamp;
}

void controller::setGps_ST(currentGPS_st var)
{
	l_currentGPS_st.latitudeGPS_float = (var).latitudeGPS_float;
	l_currentGPS_st.longitudeGPS_float = (var).longitudeGPS_float;
}

currentGPS_st controller::getGPS_ST()
{
	return l_currentGPS_st;
}

void controller::setTimestamp_U32(uint32_t timestampU32) {
	timestamp_u32 = timestampU32;
}

uint32_t controller::getTimestamp_U32() {
	return timestamp_u32;
}

stateMachine_e controller::waiting_state() {
    // find leader
    bool leader = find_leader();

    // set role
    if (leader){
        // set to leader
        role = LEADER;

    } else{
        // set to follower
        role = FOLLOWER;
    }

    // if follower connect to leader
    if(role == FOLLOWER){
        comComponent->connectToLeader(*leader_id);
        comComponent->openNewChannel(*truck_id); // for next follower
        return sm_follower_state;
    }else{
    // if follower open for connection
        comComponent->openNewChannel(*truck_id);
        return sm_leader_state;
    }
}

bool controller::find_leader(){
    bool found_leader = false;
    time_t start = time(nullptr);
    while(true){
        // find leader

        // get nearby truck list
        nearbyTruck* nearbyTruckList = comComponent->getNearbyTruckList();

        // calculate relative position (can be done on gpu)
        int self_x = location->x;
        int self_y = location->y;
        int nearest = MAX_LEADER_RANGE;

        // set the leader if the truck is infront
        for (int i = 0; i < MAX_NEARBY_TRUCK ;i++){
            //check valid truck
            if((nearbyTruckList+i)->truck_id > -1 ){
                // decide base on x position
                if((nearbyTruckList+i)->location.x>self_x){
                    found_leader =  true;
                    // get the nearest truck
                    if ((nearbyTruckList+i)->location.x-self_x<nearest) {
                        nearest = (nearbyTruckList+i)->location.x - self_x;
                        *leader_id = (nearbyTruckList+i)->truck_id;
                    }
                }
            }
        }
        if(found_leader){
            break;
        }

        // break if time out
        if (time(nullptr)-start> MAX_SEARCH_TIME){
            break;
        }
    }
    return found_leader;
}

stateMachine_e controller::moving_state(movement* signal){

    // set movement (direction and speed ) based on signal
    if (role == FOLLOWER){
        while(true){
            // validity check (should stay in move state or exit): the current movement can be overwrite by other subsystem for safety (e.g. emergency stop)
            if (signal->direction == MOVE_EMERGENCY_STOP){
                return sm_emergencyStop_state;
            }

            // change the movement of the truck if the current movement is not equal to the received signal
            if (signal->direction != current_movement.direction || signal->speed != current_movement.speed) {
                current_movement.direction = signal->direction;
                current_movement.speed = signal->speed;
                // forward message
                forwardSignal({signal->direction, signal->speed});
            }
        }

    }else{
        // as leader
        while(true){
            // validity check (should stay in move state or exit): the current movement can be overwrite by other subsystem for safety (e.g. emergency stop)
            if (current_movement.direction==MOVE_EMERGENCY_STOP){
                forwardSignal(current_movement);
                return sm_emergencyStop_state;
            }

            // drive and produce signal
            // TODO: make this fix variable dynamic base on the behaviour of the driver (driver can be human or machine)
            // these variable are depending on the behaviour of the driver (driver can be human or machine)
            movement_direction direction = MOVE_STOP;
            int speed = 0;
            current_movement={direction, speed};

            // forward
            forwardSignal(current_movement);
        }
    }
}

void controller::forwardSignal(movement signal){
    /*
     * for the received/created signal to the follower by sending the signal to truck channel
     */
    comComponent->sendMessageSelfChannel(signal);

}
