/*
 * truck.cpp
 *
 *  Created on: 22 Dec 2023
 *      Author: Luis
 */

#include "truck.h"

char inputChar = '\0';
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

truck::truck(class licensePlate licensePlate_class, class controller controller_class)
        : licensePlate_class(licensePlate_class), controller_class(controller_class)
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
                        cout << "Vehicle Direction: Forward" << endl;
                        this->controller_class.set_currentSpeed(this->controller_class.get_currentMovement().speed + this->cruiseDriverStep_u8);
                        this->controller_class.set_currentMovement(MOVE_FOWARD);
                        cout << "Vehicle Speed: " << this->controller_class.get_currentMovement().speed << endl;
                        break;
                    case 'S':
                    case 's':
                        cout << "Vehicle Direction: Stop" << endl;
                        if(this->controller_class.get_currentMovement().speed == 0)
                        {
                            this->controller_class.set_currentMovement(MOVE_STOP);
                        }
                        else
                        {
                            if(this->controller_class.get_currentMovement().speed < 10)
                            {
                                this->controller_class.set_currentSpeed(0);
                                this->controller_class.set_currentMovement(MOVE_STOP);
                            }
                            else
                            {
                                this->controller_class.set_currentSpeed(this->controller_class.get_currentMovement().speed - this->cruiseDriverStep_u8);
                                this->controller_class.set_currentMovement(MOVE_FOWARD);
                            }
                        }
                        cout << "Vehicle Speed: " << this->controller_class.get_currentMovement().speed << endl;
                        break;
                    case 'A':
                    case 'a':
                        cout << "Vehicle Direction: Left" << endl;
                        this->controller_class.currentMovement_st.direction = MOVE_LEFT;
                        break;
                    case 'D':
                    case 'd':
                        cout << "Vehicle Direction: Right" << endl;
                        this->controller_class.currentMovement_st.direction = MOVE_RIGHT;
                        break;
                    case 'H':
                    case 'h':
                        cout << "Vehicle Direction: HandBrake" << endl;
                        this->controller_class.currentMovement_st.direction = MOVE_EMERGENCY_STOP;
                        this->controller_class.currentMovement_st.speed = 0;
                        break;
                    case 'B':
                    case 'b':
                        cout << "Vehicle Direction: Brake" << endl;
                        this->controller_class.currentMovement_st.direction = MOVE_STOP;
                        this->controller_class.currentMovement_st.speed = ((this->controller_class.currentMovement_st.speed < 10) | (this->controller_class.currentMovement_st.speed <= 0)) ? (this->controller_class.currentMovement_st.speed = 0) : (this->controller_class.currentMovement_st.speed - 10);
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