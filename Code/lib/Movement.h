#ifndef MOVEMENTDIRECTION_H
#define MOVEMENTDIRECTION_H

#pragma once
enum movementDirection
{
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_EMERGENCY_STOP,
    MOVE_STOP,
};

typedef struct Movement{
    movementDirection direction;
    int speed;
}movement;




#endif