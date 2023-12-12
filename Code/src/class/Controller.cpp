/*
 * Controller.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis Fernando Rodriguez Gutierrez
 */

#include "Controller.h"

Controller::Controller(currentGPS_st varGPS, uint32_t varTimestamp)
{
	l_currentGPS_st.latitudGPS_float = varGPS.latitudGPS_float;
	l_currentGPS_st.lontgitudGPS_float = varGPS.lontgitudGPS_float;

	timespamp_u32 = varTimestamp;
}

void Controller::setGps_ST(currentGPS_st var)
{
	l_currentGPS_st.latitudGPS_float = (var).latitudGPS_float;
	l_currentGPS_st.lontgitudGPS_float = (var).lontgitudGPS_float;
}

currentGPS_st Controller::getGPS_ST()
{
	return l_currentGPS_st;
}

void Controller::setTimespamp_U32(uint32_t timespampU32) {
	timespamp_u32 = timespampU32;
}

uint32_t Controller::getTimespamp_U32() {
	return timespamp_u32;
}
