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

using namespace std;

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
    sm_systemStop_state,    //7
    sm_leader_state,        //8
    sm_follower_state,      //9
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


struct movement{
    movement_direction direction;
    int speed;
};



#endif /* COMMONLIB_H_ */
