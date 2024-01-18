#ifndef TRUCKMETADATA_H
#define TRUCKMETADATA_H

#pragma once

#include <vector>
#include "TruckRole.h"
#include "Message.h"
#include "SurroundingTruck.h"
#include "LogicalClock.h"
#include <ctime>

using namespace std;
typedef struct TruckMetadata
{

    int truck_id = -1;
    truckRole role = NOT_SET;
    logicalClock truck_logical_clock;
    int truck_leader_id = -1;
    event event_handler = ev_any;

    // Message need to be sent (update by controller, used by communication)
    vector<Message> pending_send_message;

    // Message received (update by communication, used by controller)
    vector<Message> received_message;

    // this movement used if the truck is a follower. this movement is updated by decryptor from the received message
    vector<movement> movement_leader;
    time_t watchdog;

    // list of truck -> used to find leader
    vector<SurroundingTruck> surrounding_truck; //initially -> vector<controllerSystem> vehicle_list_vector;

    // address to any other subsystems

}TruckMetadata;

#endif