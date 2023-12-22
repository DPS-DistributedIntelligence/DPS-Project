/*
 * licensePlate.h
 *
 *  Created on: 11 Dec 2023
 *      Author: Luis
 */

#ifndef LICENSEPLATE_H_
#define LICENSEPLATE_H_

#include "commonLib.h"

class licensePlate {
private:
	string cityCode_str;
	string plateNumber_str;
public:
	licensePlate(string varCityCode, string varPlateNumber);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
	string get_LicensePlate();
};

#endif /* LICENSEPLATE_H_ */
