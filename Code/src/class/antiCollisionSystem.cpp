//
// Created by Luis on 12/12/2023.
//

#include "antiCollisionSystem.h"
/*
 * Comment here
 */
antiCollisionSystem::antiCollisionSystem(float varEmergencyStopDistance_float)
{
    emergencyStopDistance_float = varEmergencyStopDistance_float;
}

float antiCollisionSystem::get_EmergencyStopDistance() {
    return emergencyStopDistance_float;
}

void antiCollisionSystem::set_EmergencyStopDistance(float emergencyStopDistanceFloat) {
    emergencyStopDistance_float = emergencyStopDistanceFloat;
}

bool antiCollisionSystem::isEmergencyStop() {
    return emergencyStop_b;
}

void antiCollisionSystem::set_EmergencyStop(bool emergencyStopB) {
    emergencyStop_b = emergencyStopB;
}
