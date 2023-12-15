/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "class/commonLib.h"
#include <time.h>

typedef struct
{
	float latitudGPS_float;
	float lontgitudGPS_float;
}currentGPS_st;

typedef enum
{
    sm_initState,          //0
    sm_waitingState,       //1
    sm_movingState,        //2
    sm_aligningState,      //3
    sm_stopState,          //4
    sm_emergencyStopState, //5
    sm_errorHandlingState, //6
    sm_systemStopState     //7
}stateMachine_e;
typedef enum
{
	leader,
	follower
}truckRole_e;

class controller {
private:
	currentGPS_st l_currentGPS_st;
    uint64_t timeStpamp_u64 = 0;
    stateMachine_e currentState_enum;
    truckRole_e role;
    uint16_t truckid_u16;
    uint16_t leaderid_u16;

    uint8_t vehicleSpeed_u8;
    uint8_t steerAngle_u8;

public:
    controller(uint16_t varTruckId, currentGPS_st varGPS);


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
	 * 	[out] stateMachine_e
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
	 * 	[out] stateMachine_e
	 */
	stateMachine_e sm_waiting_state(void);
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
	stateMachine_e sm_moving_state(void);
	/*
	 * Description:
	 * 	- Re-couple logic in case the leader was lost.
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
	 * 	[out] timespampU32
	 */
	uint64_t getTimespamp();
    /*
     * Description:
     * Parameters:
     * 	[in] vehicleSpeed
     * 	[out] null
     */
    void setVehicleSpeed(uint8_t varSpeed);
    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] vehicleSpeed
     */
    uint8_t getVehicleSpeed();

    /*
     * Description:
     *  - Angle can only be from 0-180
     *  - We consider a value of 90 as center
     *  - 0-90 is turning left
     *  - 91-180 is turning right
     * Parameters:
     * 	[in] varSteerAngle
     * 	[out] null
     */
    void setSteerAngle(uint8_t varSteerAngle);
    /*
     * Description:
     *  - Angle can only be from 0-180
     *  - We consider a value of 90 as center
     *  - 0-90 is turning left
     *  - 91-180 is turning right
     * Parameters:
     * 	[in] null
     * 	[out] varSteerAngle
     */
    uint8_t getSteerAngle();

};

#endif /* CONTROLLER_H_ */
