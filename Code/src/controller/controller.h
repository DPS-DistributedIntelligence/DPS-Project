/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "commonLib.h"

typedef struct
{
	float latitudGPS_float;
	float lontgitudGPS_float;
}currentGPS_st;

typedef enum
{
    sm_init_state,          //0
    sm_waiting_state,       //1
    sm_moving_state,        //2
    sm_aligning_state,      //3
    sm_stop_state,          //4
    sm_emergencyStop_state, //5
    sm_errorHandling_state, //6
    sm_systemStop_state     //7
}stateMachine_e;
typedef enum
{
	leader,
	follower
}truckRole_e;

class controller {
private:
	currentGPS_st l_currentGPS_st;
    uint32_t timespamp_u32 = 0;
    stateMachine_e currentState_enum;

public:
	controller(currentGPS_st varGPS, uint32_t varTimestamp);


	/*
	 * Description:
	 * Parameters:
	 * 	[in] var
	 * 	[out] null
	 */
	stateMachine_e getCurrentState(currentGPS_st var);

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
	void sm_init_state(void);

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
	void sm_waiting_state(void);
	/*
	 * Description:
	 * 	- If no LEADER was found keep role as LEADER.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	truckRole_e sm_leader_state(void);
	/*
	 * Description:
	 * 	- If LEADER was found set role as FOLLOWER.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	truckRole_e sm_follower_state(void);
	/*
	 * Description:
	 * 	- If the role is LEADER, set the pace of the speed.
	 * 	- If the role is FOLLOWER, keep the speed & distance of the LEADER.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	void sm_moving_state(truckRole_e varRole);
	/*
	 * Description:
	 * 	- Re-couple logic in case the leader was lost.
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	void sm_aligning_state(void);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	void sm_stop_state(void);
	/*
	 * Description:
	 * 	- Finish state machine
	 * Parameters:
	 * 	[in] null
	 * 	[out] null
	 */
	void sm_systemStop_state(void);



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
	void setTimespamp_U32(uint32_t timespampU32);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] timespampU32
	 * 	[out] null
	 */
	uint32_t getTimespamp_U32();


};

#endif /* CONTROLLER_H_ */
