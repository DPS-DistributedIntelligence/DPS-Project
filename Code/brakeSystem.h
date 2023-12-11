/*
 * brakeSystem.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef BRAKESYSTEM_H_
#define BRAKESYSTEM_H_

#include "Controller.h"

class brakeSystem: public Controller
{
private:
	float brakePressure_float = 0;
	bool pressureValid_b = false;
	Controller controller_class;
public:
	brakeSystem(float varBrakePressure_float, bool varPressureValid_b,
			currentGPS_st varGPS, uint32_t varTimestamp);

	/*
	 * Description:
	 * Parameters:
	 * 	[in] brakePressureFloat
	 * 	[out] null
	 */
	void setBrakePressure_Float(float brakePressureFloat);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] null
	 * 	[out] brakePressure_float
	 */
	float getBrakePressure_Float();
	/*
	 * Description:
	 * Parameters:
	 * 	[in] pressureValidB
	 * 	[out] null
	 */
	void setPressureValid_B(bool pressureValidB);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] null
	 * 	[out] pressureValid_b
	 */
	bool setPressureValid_B();
};

#endif /* BRAKESYSTEM_H_ */
