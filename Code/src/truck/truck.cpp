/*
 * truck.cpp
 *
 *  Created on: 22 Dec 2023
 *      Author: Luis
 */

#include "truck.h"

char inputChar = '\0';
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

truck::truck(licensePlate licensePlate_class, controller controllerClass)
        : licensePlate_class(licensePlate_class), controller_class(controllerClass)
{
	/*
	 * TODO: Add information from the driverInterface class for truck creation
	 */
	printf("Starting truck... \n"
			"Communication Status... \n"
			"Internal Status... \n");
}

void* truck::driverInterface(void*) {
    while (this->controller_class.get_controllerRole() == LEADER) {
        if (_kbhit())
        {
            inputChar = _getch();
            if((inputChar == 'W') | (inputChar == 'w') |
               (inputChar == 'S') | (inputChar == 's') |
               (inputChar == 'A') | (inputChar == 'a') |
               (inputChar == 'D') | (inputChar == 'd'))
            {
                pthread_mutex_lock(&mutex);
                switch (inputChar) {
                    case 'W':
                    case 'w':
                        cout << "Forward" << endl;
                        break;
                    case 'S':
                    case 's':
                        cout << "Stop" << endl;
                        break;
                    case 'A':
                    case 'a':
                        cout << "Left" << endl;
                        break;
                    case 'D':
                    case 'd':
                        cout << "Right" << endl;
                        break;
                    default:
                        cout << "IDLE" << endl;
                        break;
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    return nullptr;
}

void* truck::controller(void*) {
    while (true) {
        /*
         * TODO: Add controller logic here
         */
    }
    return nullptr;
}

void* truck::communications(void*) {
    while (true) {
        /*
         * TODO: Add communications logic here
         */
    }
    return nullptr;
}