//
// Created by Luis on 12/12/2023.
//

#include "antiCollisionSystem.h"

antiCollisionSystem::antiCollisionSystem(float varEmergencyStopDistance_float)
{
    emergencyStopDistance_float = varEmergencyStopDistance_float;
}

float antiCollisionSystem::getEmergencyStopDistance_Float() {
    return emergencyStopDistance_float;
}

void antiCollisionSystem::setEmergencyStopDistance_Float(float emergencyStopDistanceFloat) {
    emergencyStopDistance_float = emergencyStopDistanceFloat;
}

bool antiCollisionSystem::isEmergencyStop_B() {
    return emergencyStop_b;
}

void antiCollisionSystem::setEmergencyStop_B(bool emergencyStopB) {
    emergencyStop_b = emergencyStopB;
}
