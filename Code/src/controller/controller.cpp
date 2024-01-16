/*
 * controller.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */


#include "controller.h"


using namespace std;
//TODO: set function/service provider

controller::controller(uint8_t varControllerSerialNumber, bool varAnticollisionSystem, vector<controllerSystem_str> varControllerList){
    this->controllerSystem_st.controllerSerialNumber_u8 = varControllerSerialNumber;
	/* Enable of disable the controller, for this truck to be able to join the platoon
	 * the anti-collision system needs to be enable */
	this->antiCollisionSystem_class.set_EmergencyStop(varAnticollisionSystem);
    /* Copy the list of all vehicles in the controller */
    this->vehicleList_vector = varControllerList;
    this->vehicleList_vector = varControllerList;
}

stateMachine_e controller::sm_init_state()
{
    controllerSystem_st.ingnitionKey_b = true;
    /* Start the logical clock ticks */
    logicalClock_class.logicalClockTick();
    /* Store the logical clock ticks into the controller structure */
    controllerSystem_st.logicalClock_u64 = logicalClock_class.get_logicalClock();
    /* Logic to check if the logical clock was initialized though a flag */
    if(controllerSystem_st.logicalClock_u64 != 0)
    {
    	controllerSystem_st.logicalClock_b = true;
    }
    else
    {
    	controllerSystem_st.logicalClock_b = false;
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
    	 * TODO: Do Not connect truck with platoon
    	 */
    }

    return sm_waitingState;
}

/*
 * TODO: Obtain function to receive the vector of available vehicles
 */
stateMachine_e controller::waiting_state() {
    // find leader
    /* TODO: Add vector to get the leader ID */
    bool leader = find_leader();

    /* Continue the logic for the logical clock ticks */
    logicalClock_class.logicalClockTick();
    /* Store the logical clock ticks into the controller structure */
    controllerSystem_st.logicalClock_u64 = logicalClock_class.get_logicalClock();

    // set role
    if (!leader){
        // set to leader
        this->controllerSystem_st.role_e = LEADER;

    } else{
        // set to follower
    	this->controllerSystem_st.role_e = FOLLOWER;
    }

    // if follower connect to leader
    if(this->controllerSystem_st.role_e == FOLLOWER){
        return sm_followerState;
    }else{
        return sm_leaderState;
    }
}

stateMachine_e  controller::sm_leader_state()
{
    /*
     * TODO: Logic to search for the truck that will be the leader
     * Simple search of a truck, no fancy logic needed
     * Logic will need Thread implementation to be fast
     */
    // set to leader
    this->controllerSystem_st.role_e = LEADER;
    /* Continue the logic for the logical clock ticks */
    logicalClock_class.logicalClockTick();
    /* Store the logical clock ticks into the controller structure */
    controllerSystem_st.logicalClock_u64 = logicalClock_class.get_logicalClock();
    return sm_movingState;
}

stateMachine_e controller::sm_follower_state()
{
    // set to follower
    this->controllerSystem_st.role_e = FOLLOWER;
    // if follower connect to leader
    if(this->controllerSystem_st.role_e == FOLLOWER){
        if(!find_leader())
        {
            openNewChannel(truck_id); // for next follower
        }
        else
        {
            /*
             * Do Nothing
             */
        }

        return sm_movingState;
    }else{
        // if follower open for connection
        openNewChannel(truck_id);
        return sm_leaderState;
    }
}

stateMachine_e controller::sm_moving_state(){

    /*
     * TODO: set movement (direction and speed ) based on signal
     */
    if (this->controllerSystem_st.role_e == FOLLOWER){
        /* TODO: Check if this is updated to check if node is alive */
        while(true){

            if(logicalClock_class.logicalClockTickCompare(controllerSystem_st.logicalClock_u64))
            {
                /* Continue the logic for the logical clock ticks */
                logicalClock_class.logicalClockTick();
            }
            else
            {
                /* Update the logical clock of the follower */
                /*
                 * TODO: Package received from comms
                 */
                logicalClock_class.logicalClockUpdate(leaderLogicalClock);
            }

            // validity check (should stay in move state or exit): the current movement can be overwrite by other subsystem for safety (e.g. emergency stop)
            if (this->movement_st->direction == MOVE_EMERGENCY_STOP){
                return sm_emergencyStopState;
            }
            else
            {
                /*
                 * Do Nothing
                 */
            }
            // change the movement of the truck if the current movement is not equal to the received signal
            if ((this->movement_st->direction != this->currentMovement_st.direction) ||
                    (this->movement_st->speed != this->currentMovement_st.speed))
            {
                this->currentMovement_st.direction = this->movement_st->direction;
                this->currentMovement_st.speed = this->movement_st->speed;
                // forward message
                forwardSignal(this->currentMovement_st);
            }
            else
            {
                /*
                 * Do Nothing
                 */
            }
        }

    }else{
        // as leader
        while(true){
            /* TODO: Check if this is updated to check if node is alive */
            // validity check (should stay in move state or exit): the current movement can be overwrite by other subsystem for safety (e.g. emergency stop)
            if (this->currentMovement_st.direction==MOVE_EMERGENCY_STOP){
                forwardSignal(this->currentMovement_st);
                return sm_emergencyStopState;
            }

            // drive and produce signal
            // TODO: make this fix variable dynamic base on the behaviour of the driver (driver can be human or machine)
            // these variable are depending on the behaviour of the driver (driver can be human or machine)
            movement_direction direction = MOVE_STOP;
            int speed = 0;
            this->currentMovement_st = {direction, speed};

            // forward
            forwardSignal(this->currentMovement_st);
        }
    }



}

movement_str controller::get_currentMovement(void)
{
    return this->currentMovement_st;
}

void controller::set_currentMovement(movement_direction varCurrentMovement)
{
    this->currentMovement_st.direction = varCurrentMovement;
}

void controller::set_currentSpeed(uint8_t varCurrentSpeed)
{
    this->currentMovement_st.speed = varCurrentSpeed;
}

truckRole_e controller::get_controllerRole(void)
{
    return this->controllerSystem_st.role_e;
}

void controller::forwardSignal(movement_str signal){
    /*
     * for the received/created signal to the follower by sending the signal to truck channel
     */
    //TODO: forward message to communication component
    /*
     * We can send all the messages in this state or in prev, it depends on where to put logical clock
     */

}
/*
 * TODO: Move to communications
 */
bool controller::find_leader(vector<controllerSystem_str> varControllerList)
{
    bool found_leader = false;
    for(auto varControllerListTemp: varControllerList)
    {
        if(varControllerListTemp.role_e == LEADER)
        {
            found_leader = true;
            break;
        }
    }
    return found_leader;
}

uint8_t controller::get_leader(vector<controllerSystem_str> varControllerList)
{
    uint8_t leader_id = 0;
    for(auto varControllerListTemp: varControllerList)
    {
        if(varControllerListTemp.role_e == LEADER)
        {
            leader_id = varControllerListTemp.controllerSerialNumber_u8;
            break;
        }
    }
    return leader_id;
}