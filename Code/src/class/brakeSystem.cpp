/*
 * brakeSystem.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#include "brakeSystem.h"

brakeSystem::brakeSystem(float varBrakePressure_float, bool varPressureValid_b,
		currentGPS_st varGPS, uint32_t varTimestamp)
{
	Controller(varGPS, varTimestamp);
	brakePressure_float = varBrakePressure_float;
	pressureValid_b = varPressureValid_b;
}

void brakeSystem::setBrakePressure_Float(float brakePressureFloat) {
	brakePressure_float = brakePressureFloat;
}

float brakeSystem::getBrakePressure_Float() {
	return brakePressure_float;
}

void brakeSystem::setPressureValid_B(bool pressureValidB) {
	pressureValid_b = pressureValidB;
}

bool brakeSystem::setPressureValid_B() {
	return pressureValid_b;
}
