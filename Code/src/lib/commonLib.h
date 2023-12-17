/*
 * commonLib.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */
#ifndef COMMONLIB_H_
#define COMMONLIB_H_

typedef struct
{
    float latitudeGPS_float;
    float longitudeGPS_float;
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
    MOVE_FORWARD,
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

struct Location{
    int x;
    int y;
};

struct nearbyTruck{
    int truck_id;
    Location location;
};

#endif /* COMMONLIB_H_ */
