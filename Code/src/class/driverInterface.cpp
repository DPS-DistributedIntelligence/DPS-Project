/*
 * driverInterface.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#include "driverInterface.h"

driverInterface::driverInterface() {
	// TODO Auto-generated constructor stub

}

uint8_t driverInterface::get_CommunicationStatus() {
	return communicationStatus_enum;
}

void driverInterface::set_CommunicationStatus(protocolResponse varCommunicationStatusEnum) {
	communicationStatus_enum = varCommunicationStatusEnum;
}

float driverInterface::get_CurrentSpeed() {
	return currentSpeed_float;
}

void driverInterface::set_CurrentSpeed(float currentSpeedFloat) {
	currentSpeed_float = currentSpeedFloat;
}
