/*
 * driverInterface.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#ifndef DRIVERINTERFACE_H_
#define DRIVERINTERFACE_H_

#include "commonLib.h"

enum protocolResponse
{
	E_OK
};

class driverInterface {
private:
	float currentSpeed_float = 0;
	protocolResponse communicationStatus_enum = E_OK;
public:
	driverInterface();
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	uint8_t get_CommunicationStatus();
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	void set_CommunicationStatus(protocolResponse CommunicationStatusEnum);
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	float get_CurrentSpeed();
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	void set_CurrentSpeed(float currentSpeedFloat);
};

#endif /* DRIVERINTERFACE_H_ */
