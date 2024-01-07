//
// Created by Luis on 12/12/2023.
//

#include "antiCollisionSystem.h"

float antiCollisionSystem::get_EmergencyStopDistance() {
    return emergencyStopDistance_float;
}

void antiCollisionSystem::measureDistance(float varActualDistance)
{
	float actualDistanceToTruck = varActualDistance;

	if(actualDistanceToTruck < this->emergencyStopDistance_float)
	{
		this->emergencyStop_b = false;
	}
	else
	{
		this->emergencyStop_b = true;
	}
}

void antiCollisionSystem::set_EmergencyStopDistance(float emergencyStopDistanceFloat)
{
	if(this->emergencyStop_b == true)
	{
		this->emergencyStopDistance_float = emergencyStopDistanceFloat;
	}
	else
	{
		this->emergencyStopDistance_float = 0;
	}
}

bool antiCollisionSystem::isEmergencyStop() {
    return emergencyStop_b;
}

void antiCollisionSystem::set_EmergencyStop(bool emergencyStopB) {
    this->emergencyStop_b = emergencyStopB;
}
