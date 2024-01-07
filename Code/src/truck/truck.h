/*
 * truck.h
 *
 *  Created on: 22 Dec 2023
 *      Author: Luis
 */

#ifndef CLASS_TRUCK_H_
#define CLASS_TRUCK_H_

#import "../class/commonLib.h"
#import "subsystem/driverInterface.h"

typedef enum
{
	ok,
	front_axis,
	rear_axis
}truckAxis_enum;

typedef enum
{
	ok,
	frontLeft_wheelHazard,
	frontRight_wheelHazard,
	reartLeft_wheelHazard,
	rearRight_wheelHazard
}truckWheelHazard_enum;

typedef struct
{
	bool frontAxisFaulty_b, rearAxisFaulty_b;
	truckWheelHazard_enum wheelsStatus_u8;
}truckWheels_struct;

class truck {
private:
	uint8_t truckID;
public:
	uint8_t fuelTank_u8;
	licensePlate licensePlate_class;
	truckWheels_struct truckWheels_str;
	driverInterface driverInterface_class;

	truck(uint8_t varTruckSerialNumber, string varCityCode, string varPlateNumber, uint8_t varFuelTank);

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	void set_faultyFrontAxis(bool varLeftOrRight);
    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	void set_faultyRearAxis(bool varLeftOrRight);

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	truckAxis_enum get_faultyAxis();

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	truckWheelHazard_enum get_faultyWheel();

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	void set_currentFuelAmount(uint8_t varFuelAmount);

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	uint8_t get_currentFuelAmount();

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	string get_truckLicensePlate();

    /*
     * Description:
     * 	-
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
	uint8_t get_truckSerialNumber();
};

#endif /* CLASS_TRUCK_H_ */