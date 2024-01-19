#ifndef TRUCKMETADATA_H
#define TRUCKMETADATA_H
#include <mutex>
#pragma once

#include <vector>
#include "TruckRole.h"
#include "Message.h"
#include "SurroundingTruck.h"
#include "LogicalClock.h"
#include <ctime>

typedef struct TruckMetadata
{

    int truck_id = -1;
    truckRole role = NOT_SET;
    logicalClock truck_logical_clock;
    int truck_leader_id = -1;
    event event_handler = ev_any;

    // Message need to be sent (update by controller, used by communication)
    std::vector<Message> pending_send_message;
    pthread_mutex_t send_message_vector_mutex;

    // Message received (update by communication, used by controller)
    std::vector<Message> received_message;
    pthread_mutex_t received_message_vector_mutex;

    // this movement used if the truck is a follower. this movement is updated by decryptor from the received message
    std::vector<movement> movement_leader;
    pthread_mutex_t movement_leader_vec_mutex;
    time_t watchdog;

    // list of truck -> used to find leader
    pthread_mutex_t* client_IDs_vec_mutex;
    std::vector<int>* surrounding_truck_IDs;
    std::vector<SurroundingTruck> surrounding_truck; //initially -> vector<controllerSystem> vehicle_list_vector;

    // address to any other subsystems

}TruckMetadata;

#endif
