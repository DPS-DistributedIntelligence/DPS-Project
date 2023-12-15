/*
 * controller.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */


#include "controller.h"
#include "global_variable/global_variable.h"
#include <ctime>
#include <iostream>

using namespace std;
//TODO: set function/service provider
controller::controller(currentGPS_st varGPS, uint32_t varTimestamp, Truck* (*getNearbyTruck)(), Location (*getSelfLocation)(), void  (*connectToLeader)(int leaderId), void  (*openNewChannel)(int truck_id))
{
	l_currentGPS_st.latitudGPS_float = varGPS.latitudGPS_float;
	l_currentGPS_st.lontgitudGPS_float = varGPS.lontgitudGPS_float;
    this->getNearbyTruck = getNearbyTruck;
    this->getSelfLocation = getSelfLocation;
    this->connectToLeader = connectToLeader;
    this->openNewChannel = openNewChannel;

	timespamp_u32 = varTimestamp;
}

void controller::setGps_ST(currentGPS_st var)
{
	l_currentGPS_st.latitudGPS_float = (var).latitudGPS_float;
	l_currentGPS_st.lontgitudGPS_float = (var).lontgitudGPS_float;
}

currentGPS_st controller::getGPS_ST()
{
	return l_currentGPS_st;
}

void controller::setTimespamp_U32(uint32_t timespampU32) {
	timespamp_u32 = timespampU32;
}

uint32_t controller::getTimespamp_U32() {
	return timespamp_u32;
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
        this->connectToLeader;
        return sm_follower_state;
    }else{
    // if follower open for connection
        openNewChannel();
        return sm_leader_state;
    }
}

bool controller::find_leader(){
    bool found_leader = false;
    time_t start = time(nullptr);
    while(true){
        // find leader

        // get nearby truck list
        Truck* nearbyTruck = this->getNearbyTruck();

        // calculate relative position (can be done on gpu)
        int self_x = getSelfLocation()[0];
        int self_y = getSelfLocation()[1];
        int nearest = MAX_LEADER_RANGE;

        // set the leader if the truck is infront
        for (int i = 0;*(Truck+i) != NULL;i++){
            // decide base on x position
            if (*(Truck+i).location[0]>self_x){
                found_leader =  true;
                // get the nearest truck
                if (*(Truck+i).location[0]-self_x<nearest) {
                    nearest = *(Truck + i).location[0] - self_x;
                    leader_id = *(Truck + i).id;
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
