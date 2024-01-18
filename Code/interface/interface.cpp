//
// Created by Luis on 1/17/2024.
//

#include "interface.h"

char inputChar = '\0';
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void interface::driverInterface() {
    while(true)
    {
        while (self_controller.get_current_role() == LEADER) {
            if (_kbhit())
            {
                inputChar = _getch();
                if((inputChar == 'W') | (inputChar == 'w') |
                   (inputChar == 'S') | (inputChar == 's') |
                   (inputChar == 'A') | (inputChar == 'a') |
                   (inputChar == 'D') | (inputChar == 'd') |
                   (inputChar == 'E') | (inputChar == 'e') |
                   (inputChar == 'B') | (inputChar == 'b'))
                {
                    pthread_mutex_lock(&mutex);
                    switch (inputChar) {
                        case 'W':
                        case 'w':
                            if(self_controller.get_current_speed() > cruiseDriverLimit)
                            {
                                self_controller.set_current_speed(cruiseDriverLimit);
                            }
                            else
                            {
                                self_controller.set_current_speed(self_controller.get_current_speed() + cruiseDriverStep);
                            }
                            self_controller.set_current_direction(MOVE_FORWARD);
                            break;
                        case 'S':
                        case 's':
                            if(self_controller.get_current_movement().speed == 0)
                            {
                                self_controller.set_current_direction(MOVE_STOP);
                            }
                            else
                            {
                                if(self_controller.get_current_movement().speed < 10)
                                {
                                    self_controller.set_current_speed(0);
                                    self_controller.set_current_direction(MOVE_STOP);
                                }
                                else
                                {
                                    self_controller.set_current_speed(self_controller.get_current_speed() - cruiseDriverStep);
                                    self_controller.set_current_direction(MOVE_FORWARD);
                                }
                            }
                            break;
                        case 'A':
                        case 'a':
                            self_controller.set_current_direction(MOVE_LEFT);
                            break;
                        case 'D':
                        case 'd':
                            self_controller.set_current_direction(MOVE_RIGHT);
                            break;
                        case 'E':
                        case 'e':
                            self_controller.set_current_direction(MOVE_EMERGENCY_STOP);
                            self_controller.set_current_speed(0);
                            break;
                        case 'B':
                        case 'b':
                            self_controller.set_current_direction(MOVE_STOP);
                            self_controller.set_current_speed(0);
                            break;
                        default:
                            break;
                    }
                    pthread_mutex_unlock(&mutex);
                }
                else if((inputChar == 'P') | (inputChar == 'p'))
                {
                    //TODO: break server comms
                }
                else
                {
                    /*
                     * Do Nothing
                     */
                }
            }
        }
    }
}