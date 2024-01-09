/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "class/commonLib.h"
#include "subsystem/antiCollisionSystem.h"
#include "subsystem/logicalClock.h"

typedef struct
{
    union
    {
        bool ingnitionKey_b;
        bool communication_b;
        bool logicalClock_b;
        bool antiCollisionSystem_b;
    };
    uint8_t controllerSerialNumber_u8;
    truckRole_e role_e;
    uint64_t logicalClock_u64;
}controllerSystem_str;

typedef struct
{
    movement_direction direction;
    int speed;
}movemend_str;

class controller{
private:
    logicalClock logicalClock_class;
	controllerSystem_str controllerSystem_st;
	antiCollisionSystem antiCollisionSystem_class;
    /* Expected movement to follow */
    movement_str *movement_st;
    stateMachine_e currentState_enum;

    vector<controllerSystem_str> vehicleList_vector;

public:
    /* Actual movement of vehicle */
    movement_str currentMovement_st;
    /*
     * Constructor
     */
    controller(uint8_t varControllerSerialNumber, bool varAnticollisionSystem, vector<controllerSystem_str> varControllerList);

    /*
     * Description:
     * Parameters:
     * 	[in] var
     * 	[out] null
     */
    stateMachine_e getCurrentState(void);

    /*
     * Description:
     * 	This function will initiate the system. For example:
     *	- Initiate the engine.
     *	- Initiate communication system.
     *	- Set truck as leader.
     *	- TODO: Initiate all subsystems.
     *	- Initialize all variables.
     *		+ Set default values.
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_init_state(void);

    /*
     * Description:
     * 	- Start communication with nearby trucks.
     * 	- Search for a Leader Truck.
     *		+ If no LEADER was found, set truck as leader.
     *		+ If LEADER was found, set truck as follower.
     *		+ Go to the corresponding function:
     *			-> truckRole_e sm_leader_state(void)
     *			-> truckRole_e sm_follower_state(void)
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e waiting_state(void);
    /*
     * Description:
     * 	- If no LEADER was found keep role as LEADER.
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_leader_state(void);
    /*
     * Description:
     * 	- If LEADER was found set role as FOLLOWER.
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_follower_state(void);
    /*
     * Description:
     * 	- If the role is LEADER, set the pace of the speed.
     * 	- If the role is FOLLOWER, keep the speed & distance of the LEADER.
     * Parameters:
     * 	[in] truckRole_e varRole
     * 	[out] null
     */
    stateMachine_e sm_moving_state();
    /*
     * Description:
     * 	- Re-couple logic in case the leader was lost.
     * 	- moving state have dependency with the movement signal. this signal is either received from the leader or self produce if the truck is leader
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_aligning_state(void);
    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_stop_state(void);
    /*
     * Description:
     * 	- Finish state machine
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    stateMachine_e sm_systemStop_state(void);

    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] truckRole_e
     */
    truckRole_e get_controllerRole();

    /*
     * Description: this function is used to find nearest truck, and its position to be considered to be a leader
     * Parameters:
     * 	[in] null
     * 	[out] bool - found_leader
     */
    bool find_leader(vector<controllerSystem_str> varControllerList);

    /*
     * Description: This function will inform the leader ID, this will be equivalent to array position
     * Parameters:
     * 	[in] null
     * 	[out] truckRole_e - truck role
     */
    uint8_t get_leader(vector<controllerSystem_str> varControllerList);

    /*
     * Description: forward the received/created signal to the follower by sending the signal to truck channel
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void forwardSignal(movement_str signal);

    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void set_logicalClock(void);
};

#endif /* CONTROLLER_H_ */
