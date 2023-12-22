/*
 * truck.cpp
 *
 *  Created on: 22 Dec 2023
 *      Author: Luis
 */

#include "truck.h"

truck::truck(string varCityCode, string varPlateNumber, uint8_t varFuelTank)
{
	this->licensePlate_class = licensePlate(varCityCode, varPlateNumber);
	this->fuelTank_u8 = varFuelTank;
	this->truckWheels_str.frontAxisFaulty_b = false;
	this->truckWheels_str.rearAxisFaulty_b = false;

	/*
	 * TODO: Add information from the driverInterface class for truck creation
	 */
	printf("Starting truck... \n"
			"Communication Status... \n"
			"Internal Status... \n");
}

void truck::set_faultyFrontAxis(bool varLeftOrRight)
{
	this->truckWheels_str.frontAxisFaulty_b = true;
	if(varLeftOrRight == true)
	{
		this->truckWheels_str.wheelsStatus_u8 = frontLeft_wheelHazard;
	}
	else
	{
		this->truckWheels_str.wheelsStatus_u8 = frontRight_wheelHazard;
	}
}


void truck::set_faultyRearAxis(bool varLeftOrRight)
{
	this->truckWheels_str.rearAxisFaulty_b = true;
	if(varLeftOrRight == true)
	{
		this->truckWheels_str.wheelsStatus_u8 = reartLeft_wheelHazard;
	}
	else
	{
		this->truckWheels_str.wheelsStatus_u8 = rearRight_wheelHazard;
	}
}

truckAxis_enum truck::get_faultyAxis()
{
	if((this->truckWheels_str.frontAxisFaulty_b == true) or (this->truckWheels_str.rearAxisFaulty_b == true))
	{
		if(this->truckWheels_str.frontAxisFaulty_b == true)
		{
			return this->truckWheels_str.frontAxisFaulty_b;
		}
		else
		{
			return this->truckWheels_str.rearAxisFaulty_b;
		}
	}
	else
	{
		return ok;
	}
}

truckWheelHazard_enum truck::get_faultyWheel()
{
	return truckWheels_str.wheelsStatus_u8;
}

void truck::set_currentFuelAmount(uint8_t varFuelAmount)
{
	this->fuelTank_u8 = varFuelAmount;
}

uint8_t truck::get_currentFuelAmount()
{
	return this->fuelTank_u8;
}

string truck::get_truckLicensePlate()
{
	return this->get_truckLicensePlate();
}

