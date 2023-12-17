//
// Created by sheik on 12/17/2023.
//

#ifndef TEST_COMMUNICATION
#define TEST_COMMUNICATION

#include "lib/commonLib.h"

class communication{
public:
    communication();
    nearbyTruck nearbyTruckList[5]{};
    void connectToLeader(int leaderId) ;
    void openNewChannel(int truck_id);
    void sendMessageSelfChannel(movement signal);
    nearbyTruck* getNearbyTruckList();
    void sendMessageSelfChannel();

};
#endif