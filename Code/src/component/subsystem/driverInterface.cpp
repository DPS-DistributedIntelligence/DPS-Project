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

uint8_t driverInterface::getCommunicationStatus_U8() {
	return communicationStatus_enum;
}

void driverInterface::setCommunicationStatus_U8(protocolResponse varCommunicationStatusEnum) {
	communicationStatus_enum = varCommunicationStatusEnum;
}

float driverInterface::getCurrentSpeed_Float() {
	return currentSpeed_float;
}

void driverInterface::setCurrentSpeed_Float(float currentSpeedFloat = 0) {
	currentSpeed_float = currentSpeedFloat;
}
