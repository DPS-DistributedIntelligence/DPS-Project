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

controller::controller(bool varAnticollisionSystem)
{
	/* Enable of disable the controller, for this truck to be able to join the platoon
	 * the anti-collision system needs to be enable */
	this->antiCollisionSystem_class.set_EmergencyStop(varAnticollisionSystem);
	/* Set first state of the state machine to INIT */
	this->currentState_enum = sm_initState;
}

stateMachine_e controller::sm_init_state()
{
    controllerSystem_st.ingnitionKey_b = true;
    /* Each truck initializes as LEADER */
    controllerSystem_st.role_e = LEADER;
    controllerSystem_st.timespamp_u64 = getInitRTC();
    if(controllerSystem_st.timespamp_u64 != 0)
    {
    	controllerSystem_st.rtc_b = true;
    }
    else
    {
    	controllerSystem_st.rtc_b = false;
    }
    /*
     * For the anti-collision system we always need to initialize it according to diagram.
     * The distance will always be in mts.
     */
    if(controllerSystem_st.antiCollisionSystem_b == true)
    {
    	this->antiCollisionSystem_class.set_EmergencyStopDistance(10);
    }
    else
    {
    	/*
    	 * Set truck out of the platoon
    	 */
    }

    return sm_waitingState;
}

stateMachine_e controller::waiting_state() {
    // find leader
    bool leader = find_leader();

    // set role
    if (leader){
        // set to leader
        this->controllerSystem_st.role_e = LEADER;

    } else{
        // set to follower
    	this->controllerSystem_st.role_e = FOLLOWER;
    }

    // if follower connect to leader
    if(this->controllerSystem_st.role_e == FOLLOWER){
        this->connectToLeader;
        openNewChannel(truck_id); // for next follower
        return sm_followerState;
    }else{
        // if follower open for connection
        openNewChannel(truck_id);
        return sm_leaderState;
    }
}

stateMachine_e  controller::sm_leader_state()
{
    return sm_movingState;
}

stateMachine_e controller::sm_follower_state()
{
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
        openNewChannel(truck_id); // for next follower
        return sm_movingState;
    }else{
        // if follower open for connection
        openNewChannel(truck_id);
        return sm_leaderState;
    }
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

void controller::forwardSignal(movement_direction signal){
    /*
     * for the received/created signal to the follower by sending the signal to truck channel
     */
    //TODO: forward message to communication component

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

void controller::setGps_ST(currentGPS_st var)
{
    l_currentGPS_st.latitudGPS_float = (var).latitudGPS_float;
    l_currentGPS_st.lontgitudGPS_float = (var).lontgitudGPS_float;
}

currentGPS_st controller::getGPS_ST()
{
    return l_currentGPS_st;
}

uint64_t controller::getTimespamp() {
    time_t localTime_unix;
    /* Get local time un unix */
    time(&localTime_unix);

    /* Now convert to UTC */
    struct tm* timezone = gmtime(&localTime_unix);

    localTime_unix = mktime(timezone);
    this->timespamp_u64 = (uint64_t)localTime_unix;

    return this->timespamp_u64;
}

bool controller::getInitRTC()
{
    bool initRTC = false;
    initRTC = controller::getTimespamp();
    return initRTC;
}
