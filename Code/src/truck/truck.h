/*
 * truck.h
 *
 *  Created on: 22 Dec 2023
 *      Author: Luis
 */

#ifndef CLASS_TRUCK_H_
#define CLASS_TRUCK_H_

#import "class/commonLib.h"
#include "subsystem/licensePlate.h"
#include "controller/controller.h"

#include <pthread.h>
#include <unistd.h>
#include <conio.h>

class truck {
private:
    controller controller_class;
    const uint8_t cruiseDriverStep_u8 = 8;
public:
    licensePlate licensePlate_class;
	truck(licensePlate licensePlate_class, controller controllerClass);

    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void* driverInterface(void*);
    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void* controller(void*);
    /*
     * Description:
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void* communications(void*);
};

#endif /* CLASS_TRUCK_H_ */
