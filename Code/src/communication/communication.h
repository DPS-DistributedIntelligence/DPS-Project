//
// Created by sheik on 12/17/2023.
//

#ifndef TEST_COMMUNICATION
#define TEST_COMMUNICATION

#include "class/commonLib.h"

class communication{
public:
    communication();
    /* Return vehicle list by ID */
    uint8_t nearbyTruckList[5];
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void connectToLeader(uint8_t leaderId);
    /*
     * Description: Connect with truck in front of him
     * Parameters:
     * 	[in] Truck ID in front of vehicle
     * 	[out]
     */
    void openNewChannel(uint8_t truck_id);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void sendMessageSelfChannel(movement_direction signal);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    nearbyTruck* getNearbyTruckList();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void sendMessageSelfChannel();

};
#endif