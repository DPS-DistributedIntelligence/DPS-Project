//
// Created by Luis on 1/18/2024.
//
#include "Movement.h"

const char* movementDirectionStrings[] = {
        "MOVE_FORWARD",
        "MOVE_BACK",
        "MOVE_LEFT",
        "MOVE_RIGHT",
        "MOVE_EMERGENCY_STOP",
        "MOVE_STOP"
};

char* get_movement_direction_string(movementDirection varCurrentDirection)
{
    return (char*)(movementDirectionStrings[varCurrentDirection]);
}