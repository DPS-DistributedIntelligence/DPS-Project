/*
 * controller.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#include "controller.h"

//TODO: set function/service provider
controller::controller(uint16_t varTruckId_u16, currentGPS_st varGPS)
{
    this->truckid_u16 = varTruckId_u16;
    /* By default all trucks start as leader */
    this->role = leader;
    this->timeStpamp_u64 = controller::getTimespamp();
    this->l_currentGPS_st = varGPS;
}

stateMachine_e controller::sm_init_state()
{
    /* Start engine */
    /* Set timestamps */
    controller::getTimespamp();
    /* Start gps */
    controller::setGps_ST(this->l_currentGPS_st);
    /* Start comms module */
    /*
     * TODO: Set function to start communication module
     * - Constructor, funct etc...
     */

}

stateMachine_e controller::sm_moving_state()
{
    uint8_t driverInterfaceSpeed = 0;
    if(this->role == leader)
    {
        /* Set of code to allign */
        /*
         * 1. Communication is already working
         * 2. Vehicles are alligned
         */
        /* Set vehicle to n speed */
        controller::setVehicleSpeed(driverInterfaceSpeed);
        /* Prepare message to be transmitted */

        return sm_movingState;
    }
    else // follower
    {

    }
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
    this->timeStpamp_u64 = (uint64_t)localTime_unix;

    return this->timeStpamp_u64;
}

void controller::setVehicleSpeed(uint8_t varSpeed)
{
    this->vehicleSpeed_u8 = varSpeed;
}
uint8_t controller::getVehicleSpeed()
{
    return this->vehicleSpeed_u8;
}

void controller::setSteerAngle(uint8_t varSteerAngle)
{
    this->steerAngle_u8 = varSteerAngle;
}

uint8_t controller::getSteerAngle()
{
    return this->steerAngle_u8;
}