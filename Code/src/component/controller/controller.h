/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <cstdint>
#include "lib/commonLib.h"
#include "component/communication/communication.h"

class controller {
private:
	currentGPS_st l_currentGPS_st;
    uint32_t timestamp_u32 = 0;
    stateMachine_e currentState_enum;
    truckRole_e role;
    int* truck_id;
    int* leader_id;
    Location* location;
    communication* comComponent;

public:
    //controller(int truck_id, currentGPS_st varGPS, uint32_t varTimestamp, Truck* (*getNearbyTruck)(), Location (*getSelfLocation)(), void  (*connectToLeader)(int leaderId), void  (*openNewChannel)(int truck_id));

    movement current_movement = {MOVE_STOP, 0};

    /*
     *  constructor
     *  (int* truck_id,int* leader_id, Location* location, communication* comComponent)
     */
    controller(int* truck_id,int* leader_id, Location* location, communication* comComponent, currentGPS_st varGPS, uint32_t varTimestamp);

    /*
	 * Description:
	 * 	- Start communication with nearby trucks.
	 * 	- Search for a Leader Truck.
	 *		+ If no LEADER was found, set truck as leader.
	 *		+ If LEADER was found, set truck as follower.
	 *		+ Go to the corresponding function:
	 *			-> truckRole_e sm_leader_state(void)
	 *			-> truckRole_e sm_follower_state(void)
	 * Parameters: -
     * Return: next state
	 */
    stateMachine_e waiting_state();


    /*
     * Description:
     * 	- If the role is LEADER, set the pace of the speed.
     * 	- If the role is FOLLOWER, keep the speed & distance of the LEADER.
     * Parameters: -
     * Return: next state
     */
    stateMachine_e moving_state(movement* signal);

    /*
     * this function is used to find nearest truck, and its position to be considered to be a leader
     * Return: true if a leader found
     */
    bool find_leader();

    /*
     * forward the received/created signal to the follower by sending the signal to truck channel
     */
    void forwardSignal(movement signal);

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
	 * 	- If no LEADER was found keep role as LEADER.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	//stateMachine_e sm_leader_state(void);
	/*
	 * Description:
	 * 	- If LEADER was found set role as FOLLOWER.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	//stateMachine_e sm_follower_state(void);

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
	 * 	[in] null
	 * 	[out] l_currentGPS_st
	 */
	void setTimestamp_U32(uint32_t timespampU32);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] timestampU32
	 * 	[out] null
	 */
	uint32_t getTimestamp_U32();
};

#endif /* CONTROLLER_H_ */
