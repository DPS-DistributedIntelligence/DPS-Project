/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "class/commonLib.h"

class controller {
private:
    currentGPS_st l_currentGPS_st;
    uint64_t timespamp_u64 = 0;
    stateMachine_e currentState_enum;
    truckRole_e role;
    int truck_id;
    int leader_id;

    // TODO: request this functionality from communication component and other component (refer diagram)
    // required service or input
    Truck* (*getNearbyTruck)();
    Location* (*getSelfLocation)();

    /*
     * connect to a leader is finding a available channel in a channel list (provided by the simulation to te communication ) then try to access it(able to edit the array)
     */
    void  (*connectToLeader)(int leaderId);
    void  (*openNewChannel)(int truck_id);

    //
    movement current_movement = {MOVE_STOP, 0};

public:
    controller(int truck_id, currentGPS_st varGPS, uint32_t varTimestamp, Truck* (*getNearbyTruck)(), Location (*getSelfLocation)(), void  (*connectToLeader)(int leaderId), void  (*openNewChannel)(int truck_id));


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
    stateMachine_e moving_state(movement* signal);
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
     * 	[in] var
     * 	[out] null
     */
    void setGps_ST(currentGPS_st var);
    /*
     * Description:
     * Parameters:
     * 	[in] var
     * 	[out] null
     */
    currentGPS_st getGPS_ST();
    /*
     * Description:
     * Parameters:
     * 	[in] timespampU32
     * 	[out] null
     */
    uint64_t getTimespamp();

    /*
     * this function is used to find nearest truck, and its position to be considered to be a leader
     */
    bool find_leader();


    /*
     * forward the received/created signal to the follower by sending the signal to truck channel
     */
    void forwardSignal(movement_direction signal);
};

#endif /* CONTROLLER_H_ */
