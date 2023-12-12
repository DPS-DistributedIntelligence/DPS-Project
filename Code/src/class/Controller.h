/*
 * controller.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "commonLib.h"

typedef struct
{
	float latitudGPS_float;
	float lontgitudGPS_float;
}currentGPS_st;

class controller {
private:
	currentGPS_st l_currentGPS_st;
    uint32_t timespamp_u32 = 0;

public:
	controller(currentGPS_st varGPS, uint32_t varTimestamp);

	/*
	 * Description:
	 * Parameters:
	 * 	[in] var
	 * 	[out] null
	 */
	void setGps_ST(currentGPS_st var);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] var
	 * 	[out] null
	 */
	currentGPS_st getGPS_ST();
	/*
	 * Description:
	 * Parameters:
	 * 	[in] null
	 * 	[out] l_currentGPS_st
	 */
	void setTimespamp_U32(uint32_t timespampU32);
	/*
	 * Description:
	 * Parameters:
	 * 	[in] timespampU32
	 * 	[out] null
	 */
	uint32_t getTimespamp_U32();


};

#endif /* CONTROLLER_H_ */
