/*
 * driverInterface.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#ifndef DRIVERINTERFACE_H_
#define DRIVERINTERFACE_H_

#include "src/lib/commonLib.h"

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
	uint8_t getCommunicationStatus_U8();
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	void setCommunicationStatus_U8(protocolResponse CommunicationStatusEnum);
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	float getCurrentSpeed_Float();
	/*
	 * Description:
	 * Parameters:
	 * 	[in]
	 * 	[out]
	 */
	void setCurrentSpeed_Float(float currentSpeedFloat);
};

#endif /* DRIVERINTERFACE_H_ */
