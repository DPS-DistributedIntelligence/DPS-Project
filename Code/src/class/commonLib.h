/*
 * commonLib.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#ifndef COMMONLIB_H_
#define COMMONLIB_H_

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>

using namespace std;

// ANSI escape codes for text colors
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

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
    sm_systemStopState,    //7
    sm_leaderState,        //8
    sm_followerState,      //9
}stateMachine_e;

typedef enum
{
    LEADER,
    FOLLOWER
}truckRole_e;

typedef enum
{
    MOVE_FOWARD,
    MOVE_BACK,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_EMERGENCY_STOP,
    MOVE_STOP,
}movement_direction;


typedef struct {
    movement_direction direction;
    int speed;
}movement_str;

typedef struct
{
    truckRole_e vehicleRole;
    movement_str vehicleDirection;
    bool vehicleTransceiverAlive;
    bool vehicleReceiverAlive;
    uint8_t* leaderBroadcastMessage;
    uint8_t* leaderReceiveMessage;

}truckInfo_str;

#endif /* COMMONLIB_H_ */
