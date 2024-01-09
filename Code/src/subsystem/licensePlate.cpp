/*
 * licensePlate.cpp
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#include "licensePlate.h"

licensePlate::licensePlate(string varCityCode, string varPlateNumber) {
	cityCode_str = varCityCode;
	plateNumber_str = varPlateNumber;
}

string licensePlate::get_LicensePlate()
{
	return cityCode_str + " " + plateNumber_str;
}
